<?php
require_once 'config.php';

$db_host = $config['db_host'];
$db_username = $config['db_username'];
$db_password = $config['db_password'];
$db_name = $config['db_name'];

// Dados do formulário de login
$userInput = $_POST['email']; // Valor do campo de entrada do nome de usuário
$passInput = $_POST['pass']; // Valor do campo de entrada da senha

$conn = new mysqli($db_host, $db_username, $db_password, $db_name);




// Verificar se houve erro na conexão
if ($conn->connect_error) {
    die("Erro na conexão com o banco de dados: " . $conn->connect_error);
}

// Consulta SQL para verificar as credenciais
$sql = "SELECT * FROM users WHERE email = '$userInput' AND password = '$passInput'";
$result = $conn->query($sql);

// Verificar se a consulta retornou algum resultado
if ($result->num_rows > 0) {
    // Login bem-sucedido
    echo "Login correto. Redirecionando para a página correta...";
    header("Location: ../Front/home.html");
    exit; 
} else {
    // Credenciais incorretas
    echo "Credenciais incorretas. Por favor, tente novamente.";
}

// Fechar a conexão com o banco de dados
$conn->close();
?>
