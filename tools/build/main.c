#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZR_OPENING_COMMENT_TAG "/*"
#define ZR_OPENING_COMMENT_TAG_LENGTH 2
#define ZR_CLOSING_COMMENT_TAG "*/"
#define ZR_CLOSING_COMMENT_TAG_LENGTH 2

#ifdef _WIN32
#define ZR_PATH_SEPARATOR '\\'
#else
#define ZR_PATH_SEPARATOR '/'
#endif

typedef enum ZrDirectiveType { ZR_DIRECTIVE_TYPE_INCLUDE = 0 } ZrDirectiveType;

typedef struct ZrDirective ZrDirective;

typedef struct ZrIncludeDirectiveData {
    char path[512];
    char *pBuffer;
    size_t bufferSize;
} ZrIncludeDirectiveData;

struct ZrDirective {
    size_t pos;
    size_t length;
    ZrDirectiveType type;
    void *pData;
    ZrDirective *pNext;
};

static void
zrDestroyBuffer(char *pBuffer)
{
    free(pBuffer);
}

static int
zrCreateBufferFromFile(const char *pPath, size_t *pSize, void **ppBuffer)
{
    int out;
    FILE *pFile;
    long size;

    assert(pPath != NULL);
    assert(pSize != NULL);
    assert(ppBuffer != NULL);

    out = 0;

    pFile = fopen(pPath, "re");
    if (pFile == NULL) {
        fprintf(stderr, "could not open the file '%s'\n", pPath);
        out = 1;
        goto exit;
    }

    fseek(pFile, 0, SEEK_END);
    size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    *ppBuffer = malloc((size_t)size);

    if (fread(*ppBuffer, 1, (size_t)size, pFile) != (size_t)size) {
        fprintf(stderr, "could not read the file '%s'\n", pPath);
        out = 1;
        goto buffer_undo;
    }

    *pSize = (size_t)size;

    goto cleanup;

buffer_undo:
    zrDestroyBuffer(*ppBuffer);

cleanup:
    if (fclose(pFile) == EOF) {
        fprintf(stderr, "could not close the file '%s'\n", pPath);
        out = 1;
    }

exit:
    return out;
}

static int
zrWriteFile(const char *pPath, size_t size, const char *pBuffer)
{
    int out;
    FILE *pFile;

    assert(pPath != NULL);
    assert(pBuffer != NULL);

    out = 0;

    pFile = fopen(pPath, "we");
    if (pFile == NULL) {
        fprintf(stderr, "could not open the file '%s'\n", pPath);
        out = 1;
        goto exit;
    }

    if (fwrite(pBuffer, 1, size, pFile) != size) {
        fprintf(stderr, "could not write the file '%s'\n", pPath);
        out = 1;
        goto cleanup;
    }

cleanup:
    if (fclose(pFile) == EOF) {
        fprintf(stderr, "could not close the file '%s'\n", pPath);
        out = 1;
    }

exit:
    return out;
}

static const char *
zrSkipSpaces(const char *pBuffer, const char *pEnd)
{
    assert(pBuffer != NULL);
    assert(pEnd != NULL);

    while (pBuffer < pEnd && isspace(*pBuffer)) {
        ++pBuffer;
    }

    return pBuffer;
}

static size_t
zrGetWordLength(const char *pBuffer, const char *pEnd)
{
    const char *pCursor;

    assert(pBuffer != NULL);
    assert(pEnd != NULL);

    pCursor = pBuffer;
    while (pCursor < pEnd && isalnum(*pCursor)) {
        ++pCursor;
    }

    return (size_t)(pCursor - pBuffer);
}

static size_t
zrGetStringLength(const char *pBuffer, const char *pEnd)
{
    const char *pCursor;

    assert(pBuffer != NULL);
    assert(pEnd != NULL);

    pCursor = pBuffer;
    if (pCursor == pEnd || *pCursor != '"') {
        return (size_t)-1;
    }

    ++pCursor;
    while (pCursor < pEnd && *pCursor != '"') {
        ++pCursor;
    }

    if (pCursor == pEnd || *pCursor != '"') {
        return (size_t)-1;
    }

    return (size_t)(pCursor - pBuffer) - 1;
}

static void
zrDestroyIncludeDirectiveData(ZrIncludeDirectiveData *pData)
{
    if (pData == NULL) {
        return;
    }

    zrDestroyBuffer(pData->pBuffer);
    free(pData);
}

static int
zrCreateIncludeDirectiveData(const char *pBufferFront,
                             const char *pBufferBack,
                             const char *pBasePath,
                             ZrIncludeDirectiveData **ppData)
{
    int out;
    size_t length;
    size_t baseLength;
    char *pCursor;

    assert(pBufferFront != NULL);
    assert(pBufferBack != NULL);
    assert(ppData != NULL);

    out = 0;
    *ppData = (ZrIncludeDirectiveData *)malloc(sizeof(ZrIncludeDirectiveData));

    pBufferFront = zrSkipSpaces(pBufferFront, pBufferBack);
    length = zrGetStringLength(pBufferFront, pBufferBack);
    if (length == 0 || length == (size_t)-1) {
        out = 1;
        goto data_undo;
    }

    baseLength = strlen(pBasePath);
    assert(baseLength + length < sizeof(*ppData)->path);
    pCursor = (*ppData)->path;

    strncpy(pCursor, pBasePath, baseLength);
    pCursor += baseLength;

    *pCursor = ZR_PATH_SEPARATOR;
    ++pCursor;

    strncpy(pCursor, pBufferFront + 1, length);
    pCursor += length;

    *pCursor = '\0';

    if (zrCreateBufferFromFile((*ppData)->path,
                               &(*ppData)->bufferSize,
                               (void **)&(*ppData)->pBuffer)) {
        out = 1;
        goto data_undo;
    }

    goto exit;

data_undo:
    free(*ppData);

exit:
    return out;
}

static void
zrDestroyDirectives(ZrDirective *pHead)
{
    ZrDirective *pCurrent;

    while (pHead != NULL) {
        pCurrent = pHead;
        pHead = pHead->pNext;

        switch (pCurrent->type) {
            case ZR_DIRECTIVE_TYPE_INCLUDE:
                zrDestroyIncludeDirectiveData(pCurrent->pData);
                break;
            default:
                break;
        }

        free(pCurrent);
    }
}

static int
zrCreateDirectives(const char *pBuffer,
                   const char *pBasePath,
                   ZrDirective **ppHead)
{
    int out;
    ZrDirective *pCurrent;
    const char *pBufferStart;

    assert(pBuffer != NULL);
    assert(ppHead != NULL);

    out = 0;
    *ppHead = pCurrent = NULL;
    pBufferStart = pBuffer;

    while (1) {
        ZrDirective directive;
        const char *pBufferFront;
        const char *pBufferBack;
        size_t length;

        pBufferFront = strstr(pBufferStart, ZR_OPENING_COMMENT_TAG);
        if (pBufferFront == NULL) {
            break;
        }

        directive.pos = (size_t)(pBufferFront - pBuffer);

        pBufferBack = strstr(pBufferFront, ZR_CLOSING_COMMENT_TAG);
        if (pBufferBack == NULL) {
            fprintf(stderr, "no matching closing comment tag found\n");
            out = 1;
            goto directives_undo;
        }

        pBufferStart = pBufferBack + ZR_CLOSING_COMMENT_TAG_LENGTH;
        directive.length = (size_t)(pBufferStart - pBufferFront);
        pBufferFront += ZR_OPENING_COMMENT_TAG_LENGTH;

        pBufferFront = zrSkipSpaces(pBufferFront, pBufferBack);
        if (pBufferFront == pBufferBack || *pBufferFront != '@') {
            continue;
        }

        ++pBufferFront;

        pBufferFront = zrSkipSpaces(pBufferFront, pBufferBack);
        length = zrGetWordLength(pBufferFront, pBufferBack);
        if (length == 0 || length == (size_t)-1) {
            continue;
        }

        if (strncmp(pBufferFront, "include", length) == 0) {
            pBufferFront += length;
            directive.type = ZR_DIRECTIVE_TYPE_INCLUDE;
            if (zrCreateIncludeDirectiveData(
                    pBufferFront,
                    pBufferBack,
                    pBasePath,
                    (ZrIncludeDirectiveData **)&directive.pData)) {
                continue;
            }
        } else {
            continue;
        }

        directive.pNext = NULL;

        if (pCurrent == NULL) {
            *ppHead = pCurrent = (ZrDirective *)malloc(sizeof *pCurrent);
            memcpy(pCurrent, &directive, sizeof *pCurrent);
        } else {
            ZrDirective *pDirective;

            pDirective = (ZrDirective *)malloc(sizeof *pDirective);
            memcpy(pDirective, &directive, sizeof *pDirective);
            pCurrent->pNext = pDirective;
            pCurrent = pDirective;
        }
    }

    goto exit;

directives_undo:
    zrDestroyDirectives(*ppHead);

exit:
    return out;
}

static int
zrProcessIncludeDirectives(const ZrDirective *pHead,
                           size_t *pBufferSize,
                           char **ppBuffer)
{
    char *pOut;
    const ZrDirective *pCurrent;
    int doIt;
    size_t length;
    char *pFrom;
    char *pTo;
    size_t offset;

    assert(ppBuffer != NULL);
    assert(*ppBuffer != NULL);
    assert(pOut != NULL);

    doIt = 0;
    length = *pBufferSize;
    pCurrent = pHead;
    while (pCurrent != NULL) {
        if (pCurrent->type == ZR_DIRECTIVE_TYPE_INCLUDE) {
            doIt = 1;
            length += ((ZrIncludeDirectiveData *)pCurrent->pData)->bufferSize
                      - pCurrent->length;
        }

        pCurrent = pCurrent->pNext;
    }
    if (!doIt) {
        return 0;
    }

    pFrom = *ppBuffer;
    pTo = pOut = (char *)malloc(length);
    offset = 0;
    pCurrent = pHead;
    while (pCurrent != NULL) {
        if (pCurrent->type == ZR_DIRECTIVE_TYPE_INCLUDE) {
            assert(offset < *pBufferSize);
            memcpy(pTo, pFrom + offset, pCurrent->pos - offset);
            pTo += pCurrent->pos - offset;
            memcpy(pTo,
                   ((ZrIncludeDirectiveData *)pCurrent->pData)->pBuffer,
                   ((ZrIncludeDirectiveData *)pCurrent->pData)->bufferSize);
            pTo += ((ZrIncludeDirectiveData *)pCurrent->pData)->bufferSize;
            offset = pCurrent->pos + pCurrent->length;
        }

        pCurrent = pCurrent->pNext;
    }

    assert(offset < *pBufferSize);
    memcpy(pTo, pFrom + offset, *pBufferSize - offset);

    zrDestroyBuffer(*ppBuffer);
    *pBufferSize = length;
    *ppBuffer = pOut;
    return 0;
}

static void
zrGetDirectory(const char *pPath, char *pOut)
{
    char *pLastSeparator;

    pLastSeparator = strrchr(pPath, ZR_PATH_SEPARATOR);
    strncpy(pOut, pPath, (size_t)(pLastSeparator - pPath));
    pOut[pLastSeparator - pPath] = '\0';
}

int
main(int argc, char **ppArgv)
{
    int out;
    char basePath[512];
    char *pBuffer;
    size_t bufferSize;
    ZrDirective *pDirectives;

    out = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage:\n  build source destination\n");
        out = 1;
        goto exit;
    }

    assert(strlen(ppArgv[1]) < sizeof basePath);
    zrGetDirectory(ppArgv[1], basePath);

    if (zrCreateBufferFromFile(ppArgv[1], &bufferSize, (void **)&pBuffer)) {
        out = 1;
        goto exit;
    }

    if (zrCreateDirectives(pBuffer, basePath, &pDirectives)) {
        out = 1;
        goto buffer_cleanup;
    }

    if (zrProcessIncludeDirectives(pDirectives, &bufferSize, &pBuffer)) {
        out = 1;
        goto directives_cleanup;
    }

    if (zrWriteFile(ppArgv[2], bufferSize, pBuffer)) {
        out = 1;
        goto directives_cleanup;
    }

directives_cleanup:
    zrDestroyDirectives(pDirectives);

buffer_cleanup:
    zrDestroyBuffer(pBuffer);

exit:
    return out;
}
