#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define API_KEY "SUA_CHAVE_DE_API_AQUI"

int main() {
    srand(time(NULL)); // Seed aleatória para a função rand()

    // Especies de árvores disponíveis
    char *especies[] = {"Aroeira", "Ipê", "Jatobá", "Pau-Brasil", "Angico"};

    // Locais em Campinas
    char *locais[] = {"Barão Geraldo, Campinas", "Cambuí, Campinas", "Centro, Campinas", "Taquaral, Campinas", "Vila Nova, Campinas"};

    // Estados de conservação possíveis
    char *estados[] = {"Bom", "Regular", "Ruim"};

    // URL base da API de geocodificação do Google Maps
    char url_base[] = "https://maps.googleapis.com/maps/api/geocode/json?address=";

    // Inicializar a biblioteca libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Gerar 100 linhas de dados
    for (int i = 0; i < 100; i++) {
        // Gerar valores aleatórios
        char *especie = especies[rand() % 5];
        int idade = rand() % 50 + 1;
        char *local = locais[rand() % 5];
        char *endereco = local;
        char *estado = estados[rand() % 3];
        double lat = 0.0, lon = 0.0;

        // Montar a URL de requisição da API de geocodificação do Google Maps
        char url[1024];
        snprintf(url, sizeof(url), "%s%s&key=%s", url_base, endereco, API_KEY);

        // Inicializar a requisição HTTP
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Realizar a requisição HTTP
        CURLcode res = curl_easy_perform(curl);

        // Verificar se a requisição foi bem-sucedida
        if (res != CURLE_OK) {
            printf("Erro ao realizar requisição: %s\n", curl_easy_strerror(res));
            continue;
        }

        // Obter a resposta da API de geocodificação do Google Maps
        json_object *json, *results, *geometry, *location, *location_type;
        json_tokener *tokener = json_tokener_new();
        enum json_tokener_error err;
        json = json_tokener_parse_ex(tokener, curl_easy_strerror(res), strlen(curl_easy_strerror(res)));
        if ((err = json_tokener_get_error(tokener)) != json_tokener_success) {
            printf("Erro ao analisar resposta: %s\n", json_tokener_error_desc(err));
            continue;
        }

        // Procurar pelos resultados que contêm coordenadas de calçadas
        if (json_object_object_get_ex(json, "results", &results) && json_object_get_type(results) == json_type_array) {
            json_object_array_foreach(results, i, result) {
        // Verificar se o resultado contém uma geometria e se ela é do tipo "ROOFTOP" ou "RANGE_INTERPOLATED"
        if (json_object_object_get_ex(result, "geometry", &geometry) &&
            json_object_object_get_ex(geometry, "location", &location) &&
            json_object_object_get_ex(geometry, "location_type", &location_type) &&
            json_object_get_type(location_type) == json_type_string &&
            (strcmp(json_object_get_string(location_type), "ROOFTOP") == 0 || strcmp(json_object_get_string(location_type), "RANGE_INTERPOLATED") == 0)) {
            // Obter as coordenadas do resultado
            json_object *lat_obj, *lng_obj;
            if (json_object_object_get_ex(location, "lat", &lat_obj) &&
                json_object_object_get_ex(location, "lng", &lng_obj) &&
                json_object_get_type(lat_obj) == json_type_double &&
                json_object_get_type(lng_obj) == json_type_double) {
                lat = json_object_get_double(lat_obj);
                lon = json_object_get_double(lng_obj);
                break; // Parar a busca assim que uma coordenada de calçada for encontrada
            }
        }
    }
    
    // Verificar se as coordenadas foram encontradas
    if (lat == 0.0 && lon == 0.0) {
        printf("Coordenadas não encontradas para o endereço %s\n", endereco);
        continue;
    }

    // Imprimir os dados gerados
    printf("%s, %d, %lf, %lf, %s\n", especie, idade, lat, lon, estado);
}

// Liberar a memória utilizada pela resposta da API de geocodificação do Google Maps
json_object_put(json);
json_tokener_free(tokener);

// Imprimir uma mensagem de conclusão do programa
printf("Dados gerados com sucesso!\n");

// Finalizar a biblioteca libcurl
curl_global_cleanup();

return 0;