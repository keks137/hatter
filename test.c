#include "save.h"
#include "raylib.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static pthread_mutex_t storageMutex = PTHREAD_MUTEX_INITIALIZER;
static char *storageFileName = "storage.data";
char storageDataFile[256];
static bool storageInitialized = false;

bool InitStorage() {
    // pthread_mutex_lock(&storageMutex);

    if (!storageInitialized) {
        const char *basePath = GetAppStoragePath();
        if (!basePath) {
            pthread_mutex_unlock(&storageMutex);
            return false;
        }

        // Create directory hierarchy
        mkdir(basePath, 0755);

        // Build full path
        snprintf(storageDataFile, sizeof(storageDataFile), "%s/%s", basePath,
                 storageFileName);

        printf("Storage file: %s\n", storageDataFile);

        // Create empty file atomically
        FILE *file =
            fopen(storageDataFile, "ab+"); // Open for append, create if needed
        if (file) {
            fclose(file);
            storageInitialized = true;
            // pthread_mutex_unlock(&storageMutex);
            printf("hi2\n");
            return true;
        }
    }
    return false;
}

bool SaveStorageValue(unsigned int position, int value) {
    if (!storageInitialized)
        return false;
    printf("hi5\n");
    pthread_mutex_lock(&storageMutex);
    printf("hi\n");
    bool success = false;
    int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(storageDataFile, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL) {
        if (dataSize <= (position * sizeof(int))) {
            // Increase data size up to position and store value
            newDataSize = (position + 1) * sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL) {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            } else {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING,
                         "FILEIO: [%s] Failed to realloc data (%u), position "
                         "in bytes (%u) bigger than actual file size",
                         storageDataFile, dataSize, position * sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        } else {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(storageDataFile, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i",
                 storageDataFile, value);
    } else {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully",
                 storageDataFile);

        dataSize = (position + 1) * sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(storageDataFile, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i",
                 storageDataFile, value);
    }

    pthread_mutex_unlock(&storageMutex);
    return success;
}

bool SaveHighScore(int value) {
    return SaveStorageValue(STORAGE_POSITION_HISCORE, value);
}

int LoadStorageValue(unsigned int position) {
    if (!storageInitialized)
        return -1;

    printf("hi4\n");
    pthread_mutex_lock(&storageMutex);
    int value = -1;
    int dataSize = 0;

    printf("hi3\n");
    unsigned char *fileData = LoadFileData(storageDataFile, &dataSize);

    if (fileData != NULL) {
        if (dataSize < ((int)(position * 4)))
            TraceLog(LOG_WARNING,
                     "FILEIO: [%s] Failed to find storage position: %i",
                     storageDataFile, position);
        else {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i",
                 storageDataFile, value);
    }
    pthread_mutex_unlock(&storageMutex);
    return value;
}

int LoadHighScore() { return LoadStorageValue(STORAGE_POSITION_HISCORE); }
