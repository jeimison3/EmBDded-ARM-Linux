#ifndef __EXCEPTIONS_TRATAMENTO
#define __EXCEPTIONS_TRATAMENTO

    #define FUNCTION_ERROR(ERRORCODE, RAZAO) \
        fprintf(stderr, "! ERRO em \"%s\", %s(). Razão: %s.\n", __FILE__, __func__, RAZAO); \
        exit(ERRORCODE);

    #define FUNCTION_EXCEPTION(ERRORCODE) \
        fprintf(stderr, "! ERRO em \"%s\", %s() na linha %d.\n", __FILE__, __func__, __LINE__); \
        exit(ERRORCODE);
    
#endif