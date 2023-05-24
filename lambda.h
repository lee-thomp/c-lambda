#ifndef LAMBDA_H
#define LAMBDA_H

typedef enum eExprTypeE
{
    eVARIABLE = 0u,
    eLAMBDA,
    eAPPLICATION,

    eCOUNT
} eExprType_t;

typedef struct exprS
{
	eExprType_t type;
    union {
        struct {
            char *name;
        };
        struct {
            char *arg;
            struct exprS *body;
        };
        struct {
            struct exprS *term1;
            struct exprS *term2;
        };
    };
} expr_t;

#endif /* LAMBDA_H */
