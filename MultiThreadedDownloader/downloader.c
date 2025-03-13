#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curl/curl.h>
#include <string.h>

#define CHUNK_SIZE 1024 * 1024 // 1MB chunks

// Struct to store thread data
typedef struct {
    int thread_id;
    char url[2048];
    FILE *file;
    long start;
    long end;
} ThreadData;

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    return fwrite(ptr, size, nmemb, (FILE *)stream);
}

void *download_chunk(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Thread %d: Failed to initialize CURL.\n", data->thread_id);
        return NULL;
    }
    
    char range_header[128];
    snprintf(range_header, sizeof(range_header), "%ld-%ld", data->start, data->end);
    
    FILE *temp_file;
    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "temp_part_%d", data->thread_id);
    temp_file = fopen(temp_filename, "wb");
    if (!temp_file) {
        fprintf(stderr, "Thread %d: Failed to open temp file.\n", data->thread_id);
        curl_easy_cleanup(curl);
        return NULL;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, data->url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, temp_file);
    curl_easy_setopt(curl, CURLOPT_RANGE, range_header);
    
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Thread %d: Download failed: %s\n", data->thread_id, curl_easy_strerror(res));
    }
    
    fclose(temp_file);
    curl_easy_cleanup(curl);
    return NULL;
}

double get_file_size(const char *url) {
    CURL *curl = curl_easy_init();


    if (!curl) return -1;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    
    if (curl_easy_perform(curl) != CURLE_OK) {
        curl_easy_cleanup(curl);
        return -1;
    }

    
    double content_length = 0;

    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &content_length);
    curl_easy_cleanup(curl);
    printf("%d", content_length);
    return content_length;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <URL> <number_of_threads> <output_filename>\n", argv[0]);
        return 1;
    }
    
    char *url = argv[1];
    int num_threads = atoi(argv[2]);
    char *output_filename = argv[3];
    
    double file_size = get_file_size(url);
    printf("file size %d", file_size);
    if (file_size <= 0) {
        fprintf(stderr, "Failed to retrieve file size.\n");
        return 1;
    }
    
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    long chunk_size = file_size / num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        strcpy(thread_data[i].url, url);
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? file_size - 1 : (i + 1) * chunk_size - 1;
        printf("File created --> temp_file_part_%d.\n", i);
        pthread_create(&threads[i], NULL, download_chunk, &thread_data[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Downloading \n");
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        fprintf(stderr, "Failed to open output file.\n");
        return 1;
    }
    
    for (int i = 0; i < num_threads; i++) {
        char temp_filename[256];
        snprintf(temp_filename, sizeof(temp_filename), "temp_part_%d", i);
        
        FILE *temp_file = fopen(temp_filename, "rb");
        if (!temp_file) {
            fprintf(stderr, "Failed to open temp file %d.\n", i);
            continue;
        }
        
        char buffer[CHUNK_SIZE];
        size_t bytes_read;
        
        while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, temp_file)) > 0) {
            printf("Writing to main file \n");
            fwrite(buffer, 1, bytes_read, output_file);
            printf("Done \n");
        }
        
        fclose(temp_file);
        remove(temp_filename);
    }
    
    fclose(output_file);
    printf("Download completed successfully.\n");
    return 0;
}
