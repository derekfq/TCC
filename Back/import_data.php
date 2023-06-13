<?php
    require_once 'config.php';
    require 'PhpSpreadsheet/vendor/autoload.php';

    use PhpOffice\PhpSpreadsheet\IOFactory;
    use PhpOffice\PhpSpreadsheet\Writer\Csv;
    
    $arquivo = $_FILES['arquivo']['tmp_name'];
    $tipo = $_FILES['arquivo']['type'];
    
    // Verifica se um arquivo foi enviado via POST
    if (isset($_FILES['arquivo']) && $_FILES['arquivo']['error'] === UPLOAD_ERR_OK) {
        $file = $_FILES['arquivo'];
    
        // Verifica a extensão do arquivo
        $extensao = pathinfo($file['name'], PATHINFO_EXTENSION);
    
        if ($extensao == 'xlsx') {
            // Se for um arquivo XLSX, converte para CSV
            $csvFile = 'temp.csv';
    
            $objReader = IOFactory::createReader('Xlsx');
            $objReader->setReadDataOnly(true);
            $spreadsheet = $objReader->load($file['tmp_name']);
            $objWriter = new Csv($spreadsheet);
            $objWriter->setSheetIndex(0); // Define o índice da planilha desejada
            $objWriter->save($csvFile);
    
            $file['tmp_name'] = $csvFile;
            $extensao = 'csv';
        }
    
        if ($extensao == 'csv') {
            // Se for um arquivo CSV, insere as informações no banco de dados
    
            $db_host = $config['db_host'];
            $db_username = $config['db_username'];
            $db_password = $config['db_password'];
            $db_name = $config['db_name'];
            
            $conn = new mysqli($db_host, $db_username, $db_password, $db_name);
    
            // Verifica se houve erro na conexão
            if ($conn->connect_error) {
                die("Erro na conexão com o banco de dados: " . $conn->connect_error);
            }
    
            // Abre o arquivo CSV para leitura
            $handle = fopen($file['tmp_name'], "r");
    
            // Loop para ler as linhas do arquivo CSV
            while (($data = fgetcsv($handle, 1000, ",")) !== false) {
                // Insere os dados no banco de dados
                $coluna1 = $data[0]; // Exemplo de valor da primeira coluna
                $coluna2 = $data[1]; // Exemplo de valor da segunda coluna
    
                // Exemplo de consulta SQL para inserção dos dados
                $sql = "INSERT INTO tabela (coluna1, coluna2) VALUES ('$coluna1', '$coluna2')";
    
                if ($conn->query($sql) !== true) {
                    echo "Erro ao inserir os dados: " . $conn->error;
                }
            }
    
            // Fecha o arquivo CSV
            fclose($handle);
    
            // Fecha a conexão com o banco de dados
            $conn->close();
    
            // Remove o arquivo temporário CSV, se existir
            if (isset($csvFile) && file_exists($csvFile)) {
                unlink($csvFile);
            }
    
            echo "Dados inseridos com sucesso!";
        } else {
            echo "Formato de arquivo inválido. Apenas arquivos XLSX ou CSV são permitidos.";
        }
    }
    ?>