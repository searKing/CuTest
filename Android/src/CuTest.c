#include <assert.h>
#include <setjmp.h>
#include <string.h>
#include <math.h>

#include "CuTest.h"
/*
 *open it when in debug mode, closed it in release mode
 *added by chenhaixin
 *2014-12-17
 */
#ifdef DEBUG_LOG
#include <stdio.h>
#include <stdlib.h>
#include<android/log.h>
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "FUNC; [%s] line [%d]\t... "fmt, __FUNCTION__,__LINE__,##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "FUNC; [%s] line [%d]\t... "fmt, __FUNCTION__,__LINE__,##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "FUNC; [%s] line [%d]\t... "fmt, __FUNCTION__,__LINE__,##args)
#else
#include <stdio.h>
#include <stdlib.h>
#include<android/log.h>
#define LOGI(fmt, args...)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "FUNC; [%s] line [%d]\t... "fmt, __FUNCTION__,__LINE__,##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "FUNC; [%s] line [%d]\t... "fmt, __FUNCTION__,__LINE__,##args)
#endif
//--In this CuTest.c, we use ShallowCopy at most,but DeepOperation Default
//--XXXAlloc:Alloc a continued memory
//--XXXDeepCopy:Alloc a new addr and Copy the context of the buffer
//--XXXShallowCopy:just Copy the addr
//--XXXInit:Alloc the XXX's own buffer and Init
//--XXXNew:Alloc the XXX and it's own buffer and Init
//--XXXDeepDelete:Free the XXX and it's own buffer
//--XXXShallowDelete:just Free the XXX
//--XXXResize:Realloc a XXX's own buffer, as alloc a new buff and deep copy from the old buf, free the old finally
/*-------------------------------------------------------------------------*
 * CuStr --a pure array of char ended with '/0'
 *-------------------------------------------------------------------------*/


/***************************************************************
   Function:	CuStrAlloc
   Description: Alloc a continued memory
   param:       size - byte size
   Returns:     mem addr alloced in heap
 ****************************************************************/
char *CuStrAlloc(int size)
{
    LOGD("CuStrAlloc() Entered!\n");
    char *newStr = (char *) malloc( sizeof(char) * (size) );
    LOGD("CuStrAlloc() Finished!\n");
    return newStr;
}


/***************************************************************
   Function:	CuStrAlloc
   Description: new a memery and copy from old addr
   param:       old - src mem
   Returns:     mem addr alloced in heap with a full copy from old
 ****************************************************************/
char *CuStrDeepCopy(const char *old)
{
    LOGD("CuStrDeepCopy() Entered!\n");
    if(!old)
    {
        LOGE("Input Param is NULL!\n");
        return NULL;
    }
    int len = strlen(old);
    char *newStr = CuStrAlloc(len + 1);
    strcpy(newStr, old);
    LOGD("CuStrDeepCopy() Finished!\n");
    return newStr;
}

/***************************************************************
   Function:	CuStrFree
   Description: release the memery in the heap
   param:       str - pointer to a memory in the heap
   Returns:     null
 ****************************************************************/
void CuStrFree(char *str)
{
    LOGD("CuStrFree() Entered!\n");
    if(!str)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    free(str);
    LOGD("CuStrFree() Finished!\n");
}
/*-------------------------------------------------------------------------*
 * CuString
 *-------------------------------------------------------------------------*/

/***************************************************************
   Function:	CuStringConstructor
   Description: CuString(), as in C++
   param:       str - as this in C++
   Returns:     null
 ****************************************************************/
void CuStringConstructor(CuString *str)
{
    LOGD("CuStringConstructor() Entered!\n");
    if(!str)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char *) malloc(sizeof(char) * str->size);
    str->buffer[0] = '\0';
    LOGD("CuStringConstructor() Finished!\n");
}

/***************************************************************
   Function:	CuStringNew
   Description: new CuString, as in C++
   Returns:     as this in C++
 ****************************************************************/
CuString *CuStringNew(void)
{
    LOGD("CuStringNew() Entered!\n");
    CuString *str = (CuString *) malloc(sizeof(CuString));
    CuStringConstructor(str);
    LOGD("CuStringNew() Finished!\n");
    return str;
}

/***************************************************************
   Function:	CuStringDestructor
   Description: ~CuString(), as in C++
   param:       str - as this in C++
   Returns:     null
 ****************************************************************/
void CuStringDestructor(CuString *str)
{
    LOGD("CuStringDestructor() Entered!\n");
    if(!str)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    CuStrFree(str->buffer);
    str->buffer = NULL;
    LOGD("CuStringDestructor() Finished!\n");
}

/***************************************************************
   Function:	CuStringDelete
   Description: delete CuString, as in C++
   param:       str - as this in C++
   Returns:     null
 ****************************************************************/
void CuStringDelete(CuString *str)
{
    LOGD("CuStringDelete() Entered!\n");
    if(!str)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    CuStringDestructor(str);
    free(str);
    LOGD("CuStringDelete() Finished!\n");
}

/***************************************************************
   Function:	CuStringResize
   Description: resize CuString, as a member function in C++
   param:       str 	- as this in C++
   				newSize - new mem size of member variable buffer
   Returns:     null
 ****************************************************************/
void CuStringResize(CuString *str, int newSize)
{
    LOGD("CuStringResize() Entered!\n");
    if(!str)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    str->buffer = (char *) realloc(str->buffer, sizeof(char) * newSize);
    str->size = newSize;
    LOGD("CuStringResize() Finished!\n");
}

/***************************************************************
   Function:	CuStringAppend
   Description: append string by the end of buffer in CuString, as a member function in C++
   param:       str 	- as this in C++
   				text 	- appended string
   Returns:     null
 ****************************************************************/
void CuStringAppend(CuString *str, const char *text)
{
    LOGD("CuStringAppend() Entered!\n");
    int length;
    if(!str || !text)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    length = strlen(text);

    if (str->length + length + 1 >= str->size)
        CuStringResize(str, str->length + length + 1 + STRING_INC);

    str->length += length;
    strcat(str->buffer, text);
    LOGD("CuStringAppend() Finished!\n");
}

/***************************************************************
   Function:	CuStringAppendChar
   Description: append char by the end of buffer in CuString, as a member function in C++
   param:       str 	- as this in C++
   				text 	- appended string
   Returns:     null
 ****************************************************************/
void CuStringAppendChar(CuString *str, char ch)
{
    LOGD("CuStringAppendChar() Entered!\n");
    if(!str)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    char text[2];
    text[0] = ch;
    text[1] = '\0';
    CuStringAppend(str, text);
    LOGD("CuStringAppendChar() Finished!\n");
}

/***************************************************************
   Function:	CuStringAppendFormat
   Description: append a formatted string as printf used by the end of buffer in CuString, as a member function in C++
   param:       str 	- as this in C++
   				format 	- appended fromated string
   Returns:     null
 ****************************************************************/
void CuStringAppendFormat(CuString *str, const char *format, ...)
{
    LOGD("CuStringAppendFormat() Entered!\n");
    if(!str || !format)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    va_list argp;
    char buf[HUGE_STRING_LEN];
    va_start(argp, format);
    vsprintf(buf, format, argp);
    va_end(argp);
    CuStringAppend(str, buf);
    LOGD("CuStringAppendFormat() Finished!\n");
}

/***************************************************************
   Function:	CuStringInsert
   Description: insert a string before the pos in the buffer in CuString, as a member function in C++
   param:       str 	- as this in C++
   				text 	- inserted string
   				pos		- insert postion
   Returns:     null
 ****************************************************************/
void CuStringInsert(CuString *str, const char *text, int pos)
{
    LOGD("CuStringInsert() Entered!\n");

    if(!str || !text)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    int length = strlen(text);

    if (pos > str->length)
        pos = str->length;

    if (str->length + length + 1 >= str->size)
        CuStringResize(str, str->length + length + 1 + STRING_INC);

    memmove(str->buffer + pos + length, str->buffer + pos, (str->length - pos) + 1);
    str->length += length;
    memcpy(str->buffer + pos, text, length);
    LOGD("CuStringInsert() Finished!\n");
}

/*-------------------------------------------------------------------------*
 * CuTest
 *-------------------------------------------------------------------------*/


/***************************************************************
   Function:	CuTestConstructor
   Description: CuTest(), as in C++
   param:       tc 		- as this in C++
   				name	- name of this test unit
   				function- pointer to the function which will be tested
   Returns:     null
 ****************************************************************/
void CuTestConstructor(CuTest *tc, const char *name, TestFunction function)
{
    LOGD("CuTestConstructor() Entered!\n");
    if(!tc || !name)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    tc->name = CuStrDeepCopy(name);
    tc->failed = 0;
    tc->ran = 0;
    tc->message = NULL;
    tc->function = function;
    tc->jumpBuf = NULL;
    LOGD("CuTestConstructor() Finished!\n");
}

/***************************************************************
   Function:	CuTestNew
   Description: new CuTest(name, function), as in C++
   param:       name	- name of this test unit
   				function- pointer to the function which will be tested
   Returns:     as this in C++
 ****************************************************************/
CuTest *CuTestNew(const char *name, TestFunction function)
{
    LOGD("CuTestNew() Entered!\n");
    if(!name)
    {
        LOGE("Input Param is NULL!\n");
        return NULL;
    }
    CuTest *tc = CU_ALLOC(CuTest);
    CuTestConstructor(tc, name, function);
    LOGD("CuTestNew() Finished!\n");
    return tc;
}

/***************************************************************
   Function:	CuTestDestructor
   Description: ~CuTest(), as in C++
   param:       tc - as this in C++
   Returns:     null
 ****************************************************************/
void CuTestDestructor(CuTest *tc)
{
    LOGD("CuTestDestructor() Entered!\n");

    if(!tc)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    CuStrFree(tc->name);
    tc->name = NULL;
    LOGD("CuTestDestructor() Finished!\n");
}

/***************************************************************
   Function:	CuTestDelete
   Description: delete CuTest, as in C++
   param:       tc - as this in C++
   Returns:     null
 ****************************************************************/
void CuTestDelete(CuTest *tc)
{
    LOGD("CuTestDelete() Entered!\n");

    if(!tc)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    CuTestDestructor(tc);
    free(tc);
    LOGD("CuTestDelete() Finished!\n");
}
/***************************************************************
   Function:	CuTestRun
   Description: run CuTest, as a member function in C++
   param:       tc 	- as this in C++
   Returns:     null
 ****************************************************************/
void CuTestRun(CuTest *tc)
{
    LOGD("CuTestRun() Entered, tc(%p)!\n", tc);

    if(!tc)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    jmp_buf buf;
    tc->jumpBuf = &buf;

    if (setjmp(buf) == 0)
    {
        tc->ran = 1;
        //longjmp will be called in tc->function
        (tc->function)(tc);
    }
    else
    {
        LOGD("CuTestRun() ReEntered with longjmp(ptr %#x)!\n", (unsigned int)tc->jumpBuf);
    }

    tc->jumpBuf = 0;
    LOGD("CuTestRun() Finished!\n");
}

/***************************************************************
   Function:	CuTestFailInternal
   Description: record the error tips, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				string	- error tips
   Returns:     null
 ****************************************************************/
void CuTestFailInternal(CuTest *tc, const char *file, int line, CuString *string)
{
    LOGD("CuTestFailInternal() Entered!\n");
    char buf[HUGE_STRING_LEN];

    if(!tc || !file || !string)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    sprintf(buf, "%s:%d: ", file, line);
    CuStringInsert(string, buf, 0);
    tc->failed = 1;
    tc->message = string->buffer;

    if (tc->jumpBuf != 0)
    {
        LOGD("CuTestFailInternal() Finished with longjmp(ptr %#x)!\n", (unsigned int)tc->jumpBuf);
        longjmp(*(tc->jumpBuf), 0);
    }
    LOGD("CuTestFailInternal() Finished!\n");
}

/***************************************************************
   Function:	CuTestFail_Line
   Description: record the error tips, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				errorType		- error tips type
   				errorContent	- error tips type
   Returns:     null
 ****************************************************************/
void CuTestFail_Line(CuTest *tc, const char *file, int line, const char *errorType, const char *errorContent)
{
    LOGD("CuTestFail_Line() Entered!\n");
    CuString string;
    if(!tc || !file || !errorContent)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    CuStringConstructor(&string);

    if (errorType != NULL)
    {
        CuStringAppend(&string, errorType);
        CuStringAppend(&string, ": ");
    }

    CuStringAppend(&string, errorContent);
    CuTestFailInternal(tc, file, line, &string);
    CuStringDestructor(&string);
    LOGD("CuTestFail_Line() Finished!\n");
}

/***************************************************************
   Function:	CuTestAssert_Line
   Description: record error tips if assert condition is false, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				message		- error tips type
   				condition	- 0:fail and record; else:true and return
   Returns:     null
 ****************************************************************/
void CuTestAssert_Line(CuTest *tc, const char *file, int line, const char *message, int condition)
{
    LOGD("CuTestAssert_Line() Entered!\n");
    if(!tc || !file || !message)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    if (condition)
    {
        return;
    }

    CuTestFail_Line(tc, file, line, NULL, message);
    LOGD("CuTestAssert_Line() Finished!\n");
}

/***************************************************************
   Function:	CuTestAssertStrEquals_LineMsg
   Description: record error tips if strings is not equal, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				message	- error tips
   				expected- expected string
   				actual	- actual string
   Returns:     null
 ****************************************************************/
void CuTestAssertStrEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                                   const char *expected, const char *actual)
{
    LOGD("CuTestAssertStrEquals_LineMsg() Entered!\n");
    CuString string;
    if(!tc || !file || !message)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    if ((expected == NULL && actual == NULL) ||
            (expected != NULL && actual != NULL &&
             strcmp(expected, actual) == 0))
    {
        return;
    }

    CuStringConstructor(&string);

    if (message != NULL)
    {
        CuStringAppend(&string, message);
        CuStringAppend(&string, ": ");
    }

    CuStringAppend(&string, "expected <");
    CuStringAppend(&string, expected);
    CuStringAppend(&string, "> but was <");
    CuStringAppend(&string, actual);
    CuStringAppend(&string, ">");
    CuTestFailInternal(tc, file, line, &string);
    LOGD("CuTestAssertStrEquals_LineMsg() Finished!\n");
}

/***************************************************************
   Function:	CuTestAssertIntEquals_LineMsg
   Description: record error tips if strings is not equal, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				message	- error tips
   				expected- expected int
   				actual	- actual int
   Returns:     null
 ****************************************************************/
void CuTestAssertIntEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                                   int expected, int actual)
{
    LOGD("CuTestAssertIntEquals_LineMsg() Entered!\n");
    char buf[STRING_MAX];
    if(!tc || !file || !message)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    if (expected == actual)
    {
        return;
    }

    sprintf(buf, "expected <%d> but was <%d>", expected, actual);
    CuTestFail_Line(tc, file, line, message, buf);
    LOGD("CuTestAssertIntEquals_LineMsg() Finished!\n");
}

/***************************************************************
   Function:	CuTestAssertDblEquals_LineMsg
   Description: record error tips if strings is not equal, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				message	- error tips
   				expected- expected double
   				actual	- actual double
   				delta	- error threshold
   Returns:     null
 ****************************************************************/
void CuTestAssertDblEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                                   double expected, double actual, double delta)
{
    LOGD("CuTestAssertDblEquals_LineMsg() Entered!\n");
    char buf[STRING_MAX];

    if(!tc || !file || !message)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    if (fabs(expected - actual) <= delta)
    {
        return;
    }

    sprintf(buf, "expected <%f> but was <%f>", expected, actual);
    CuTestFail_Line(tc, file, line, message, buf);
    LOGD("CuTestAssertDblEquals_LineMsg() Finished!\n");
}

/***************************************************************
   Function:	CuTestAssertPtrEquals_LineMsg
   Description: record error tips if strings is not equal, as a member function in C++
   param:       tc 		- as this in C++
   				file	- filename when error happends
   				line	- lineno when error happends
   				message	- error tips
   				expected- expected ptr
   				actual	- actual ptr
   Returns:     null
 ****************************************************************/
void CuTestAssertPtrEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                                   void *expected, void *actual)
{
    LOGD("CuTestAssertPtrEquals_LineMsg() Entered!\n");
    char buf[STRING_MAX];

    if(!tc || !file || !message)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    if (expected == actual)
    {
        return;
    }

    sprintf(buf, "expected pointer <%p> but was <%p>", expected, actual);
    CuTestFail_Line(tc, file, line, message, buf);
    LOGD("CuTestAssertPtrEquals_LineMsg() Finished!\n");
}


/*-------------------------------------------------------------------------*
 * CuSuite
 *-------------------------------------------------------------------------*/

/***************************************************************
   Function:	CuSuiteConstructor
   Description: CuSuite(), as in C++
   param:       testSuite	- as this in C++
   Returns:     null
 ****************************************************************/
void CuSuiteConstructor(CuSuite *testSuite)
{
    LOGD("CuSuiteConstructor() Entered!\n");
    if(!testSuite)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    testSuite->count = 0;
    testSuite->failCount = 0;
    memset(testSuite->list, 0, sizeof(testSuite->list));
    LOGD("CuSuiteConstructor() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteNew
   Description: new CuSuite(name, function), as in C++
   param:
   Returns:     as this in C++
 ****************************************************************/
CuSuite *CuSuiteNew(void)
{
    LOGD("CuSuiteNew() Entered!\n");
    CuSuite *testSuite = CU_ALLOC(CuSuite);
    CuSuiteConstructor(testSuite);
    LOGD("CuSuiteNew() Finished!\n");
    return testSuite;
}

/***************************************************************
   Function:	CuSuiteDestructor
   Description: ~CuSuite(), as in C++
   param:       testSuite - as this in C++
   Returns:     null
 ****************************************************************/
void CuSuiteDestructor(CuSuite *testSuite)
{
    LOGD("CuSuiteDestructor() Entered!\n");

    if(!testSuite)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    int n;

    for (n = 0; n < testSuite->count; n++)
    {
        if (testSuite->list[n])
        {
            CuTestDelete(testSuite->list[n]);
            testSuite->list[n] = NULL;
        }
    }
    testSuite->count = 0;
    testSuite->failCount = 0;
    LOGD("CuSuiteDestructor() Finished!\n");
}
/***************************************************************
   Function:	CuSuiteDelete
   Description: delete CuSuite, as in C++
   param:       testSuite - as this in C++
   Returns:     null
 ****************************************************************/
void CuSuiteDelete(CuSuite *testSuite)
{
    LOGD("CuSuiteDelete() Entered!\n");

    if(!testSuite)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    CuSuiteDestructor(testSuite);
    free(testSuite);
    LOGD("CuSuiteDelete() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteShallowAdd
   Description: add a new testCase shallowly , just take the testCase, as a member function in C++
   param:       testSuite 	- as this in C++
   				testCase 	- new testCase
   Returns:     null
 ****************************************************************/
void CuSuiteShallowAdd(CuSuite *testSuite, CuTest *testCase)
{
    LOGD("CuSuiteShallowAdd() Entered!\n");
    if(!testSuite || !testCase)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    assert(testSuite->count < MAX_TEST_CASES);
    testSuite->list[testSuite->count] = testCase;
    testSuite->count++;
    LOGD("CuSuiteShallowAdd() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteDeepAdd
   Description: add a new testCase deeply, new a testCase and copy into it , as a member function in C++
   param:       testSuite 	- as this in C++
   				testCase 	- new testCase
   Returns:     null
 ****************************************************************/
void CuSuiteDeepAdd(CuSuite *testSuite, CuTest *testCase)
{
    LOGD("CuSuiteDeepAdd() Entered!\n");
    if(!testSuite || !testCase)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }
    assert(testSuite->count < MAX_TEST_CASES);
    CuTest *tc = CU_ALLOC(CuTest);
    memcpy(tc, testCase, sizeof(*testCase));
    testSuite->list[testSuite->count] = tc;
    testSuite->count++;
    LOGD("CuSuiteDeepAdd() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteShallowAdd
   Description: rob the testCases shallowly from another testSuite, as a member function in C++
				in this program ,
				if A is added with B,and A&B is the same type,
				then B is robbed by A
				and B lost B's buffer loaded
				it's caused by the diff of shallow/deep copy, which can be solved by ref count
				generally,we don't use deep copy under the consideration of performance, geeks konw
   param:       testSuite 	- as this in C++
   				testCase 	- new testCase
   Returns:     null
 ****************************************************************/
void CuSuiteRobSuite(CuSuite *testSuite, CuSuite *testSuiteRobbed)
{
    LOGD("CuSuiteRobSuite() Entered!\n");
    int i;
    if(!testSuite || !testSuiteRobbed)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    for (i = 0 ; i < testSuiteRobbed->count ; ++i)
    {
        CuTest *testCase = testSuiteRobbed->list[i];
        CuSuiteShallowAdd(testSuite, testCase);
    }
    //testSuite2->list is robbed, to avoid testSuite2->list[idx] be freeed twice
    testSuiteRobbed->count = 0;
    memset(testSuiteRobbed->list, sizeof(testSuiteRobbed->list));
    LOGD("CuSuiteRobSuite() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteRun
   Description: run CuSuite, as a member function in C++
   param:       testSuite 	- as this in C++
   Returns:     null
 ****************************************************************/
void CuSuiteRun(CuSuite *testSuite)
{
    LOGD("CuSuiteRun() Entered!\n");
    int i;
    if(!testSuite)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    for (i = 0 ; i < testSuite->count ; ++i)
    {
        LOGD("testSuite->idx/count =%d/%d Entered!\n", i, testSuite->count);
        CuTest *testCase = testSuite->list[i];
        if(!testCase) continue;
        CuTestRun(testCase);

        if (testCase->failed)
        {
            testSuite->failCount += 1;
        }
    }
    LOGD("CuSuiteRun() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteSummary
   Description: generate the simple test result of all testCases
   param:       testSuite 	- as this in C++
   				summary		- the test result of all testCases
   Returns:     null
 ****************************************************************/
void CuSuiteSummary(CuSuite *testSuite, CuString *summary)
{
    LOGD("CuSuiteSummary() Entered!\n");
    int i;
    if(!testSuite || !summary)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    for (i = 0 ; i < testSuite->count ; ++i)
    {
        CuTest *testCase = testSuite->list[i];
        CuStringAppend(summary, testCase->failed ? "F" : ".");
    }

    CuStringAppend(summary, "\n\n");
    LOGD("CuSuiteSummary() Finished!\n");
}

/***************************************************************
   Function:	CuSuiteDetails
   Description: generate the detail test result of all testCases
   param:       testSuite 	- as this in C++
   				summary		- the test result of all testCases
   Returns:     null
 ****************************************************************/
void CuSuiteDetails(CuSuite *testSuite, CuString *details)
{
    LOGD("CuSuiteDetails() Entered!\n");
    int i;
    int failCount = 0;
    if(!testSuite || !details)
    {
        LOGE("Input Param is NULL!\n");
        return;
    }

    if (testSuite->failCount == 0)
    {
        for (i = 0 ; i < testSuite->count ; ++i)
        {
            CuTest *testCase = testSuite->list[i];
            CuStringAppendFormat(details, "%d/%d) [%s]%s\n",
                                 i,
                                 testSuite->count,
                                 (testCase->failed ? "F" : "."),
                                 testCase->name);
        }
        int passCount = testSuite->count - testSuite->failCount;
        const char *testWord = passCount == 1 ? "test" : "tests";
        CuStringAppendFormat(details, "OK (%d %s)\n", passCount, testWord);
    }
    else
    {
        if (testSuite->failCount == 1)
            CuStringAppend(details, "There was 1 failure:\n");
        else
            CuStringAppendFormat(details, "There were %d failures:\n", testSuite->failCount);

        for (i = 0 ; i < testSuite->count ; ++i)
        {
            CuTest *testCase = testSuite->list[i];

            if (testCase->failed)
            {
                failCount++;
                CuStringAppendFormat(details, "%d) %s: %s\n",
                                     failCount, testCase->name, testCase->message);
            }
        }

        CuStringAppend(details, "\n!!!FAILURES!!!\n");
        CuStringAppendFormat(details, "Runs: %d ",   testSuite->count);
        CuStringAppendFormat(details, "Passes: %d ", testSuite->count - testSuite->failCount);
        CuStringAppendFormat(details, "Fails: %d\n",  testSuite->failCount);
    }
    LOGD("CuSuiteDetails() Finished!\n");
}
