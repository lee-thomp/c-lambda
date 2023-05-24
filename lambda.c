/**
 * @file
 * @brief 	Lambda interpreter.
 *
 * 		Details
 */

/*============================== includes ==============================*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lambda.h"

/*============================== macros ==============================*/

/* Macro magic for better printf debugging. */
#define DEBUG_PRINT_VAL_NAME_LENGTH 	(32)
#define DEBUG_PRINT(type, val)                                          \
    do {                                                                \
        assert(DEBUG_PRINT_VAL_NAME_LENGTH > sizeof(#val));             \
        printf("%-*s=> " type "\n", DEBUG_PRINT_VAL_NAME_LENGTH, #val, val); \
    } while (0);

#define NOT_IMPLEMENTED                                                 \
    fprintf(stderr, "NOT IMPLEMENTED AT LINE %d IN %s", __LINE__, __FILE__)

/*============================== typedef ==============================*/


/*============================== globals ==============================*/


/*============================== statics ==============================*/


/*============================== functions ==============================*/
char *const ShowExpr (expr_t const *const expr)
{
    assert(NULL != expr);
    assert(eCOUNT > expr->type);

    char *showBuf = NULL;

	switch (expr->type % eCOUNT)
    {
        case eVARIABLE:
        {
            showBuf = (char *)malloc(sizeof("") + strlen(expr->name));
            sprintf(showBuf, "%s", expr->name);
            break;
        }

        case eLAMBDA:
        {
            char *bodyDisplay = ShowExpr(expr->body);

            size_t argLen = strlen(expr->arg);
            size_t bodyLen = strlen(bodyDisplay);

            showBuf = (char *)malloc(sizeof("(λ () )") + argLen + bodyLen);

            sprintf(showBuf, "(λ (%s) %s)", expr->arg, bodyDisplay);

            free(bodyDisplay);
            break;
        }

        case eAPPLICATION:
        {
            char *term1Display = ShowExpr(expr->term1);
            char *term2Display = ShowExpr(expr->term2);

            size_t term1Len = strlen(term1Display);
            size_t term2Len = strlen(term2Display);

            showBuf = (char *)malloc(sizeof("( )") + term1Len + term2Len);

            sprintf(showBuf, "(%s %s)", term1Display, term2Display);

            free(term1Display);
            free(term2Display);
            break;
        }

        default:
        {
            /* Halt and catch fire. */
            raise(SIGTRAP);
            break;
        }
    }
    return (NULL != showBuf) ? showBuf : (char *)malloc(0);
}

/*============================== main ==============================*/

int main (void)
{
    expr_t testApp =
        {.type = eAPPLICATION,
         .term1 = &(expr_t){.type = eVARIABLE, .name = "func"},
         .term2 = &(expr_t){.type = eVARIABLE, .name = "arg"}
        };

    expr_t testLam =
        {.type = eLAMBDA,
         .arg = "x",
         .body = &(expr_t){.type = eVARIABLE, .name = "y"}
        };

    expr_t testK =
        {.type = eLAMBDA,
         .arg = "x",
         .body =
         &(expr_t)
         {.type = eLAMBDA,
          .arg = "y",
          .body =
          &(expr_t)
          {.type = eVARIABLE,
           .name = "x"}}
        };

    char *lambdaDisplay = ShowExpr(&testApp);
    
    printf("%s\n", lambdaDisplay);
    free(lambdaDisplay);
    
    lambdaDisplay = ShowExpr(&testLam);
    
    printf("%s\n", lambdaDisplay);
    free(lambdaDisplay);
    
    lambdaDisplay = ShowExpr(&testK);
    
    printf("%s\n", lambdaDisplay);
    free(lambdaDisplay);
    
	return 0;
}

