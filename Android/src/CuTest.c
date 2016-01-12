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

char *CuStrAlloc(int size)
{
    LOGD("CuStrAlloc() Entered!\n");
    char *newStr = (char *) malloc( sizeof(char) * (size) );
    LOGD("CuStrAlloc() Finished!\n");
    return newStr;
}

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

CuString *CuStringNew(void)
{
    LOGD("CuStringNew() Entered!\n");
    CuString *str = (CuString *) malloc(sizeof(CuString));
    CuStringConstructor(str);
    LOGD("CuStringNew() Finished!\n");
    return str;
}

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
void CuTestRun(CuTest *tc)
{
    LOGD("CuTestRun() Entered, tc(%#X)!\n",(unsigned int)tc);
	
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
    	LOGD("CuTestRun() ReEntered with longjmp(ptr %#x)!\n",(unsigned int)tc->jumpBuf);			
	}

    tc->jumpBuf = 0;
    LOGD("CuTestRun() Finished!\n");
}

static void CuFailInternal(CuTest *tc, const char *file, int line, CuString *string)
{
    LOGD("CuFailInternal() Entered!\n");
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
    	LOGD("CuFailInternal() Finished with longjmp(ptr %#x)!\n",(unsigned int)tc->jumpBuf);
		longjmp(*(tc->jumpBuf), 0);
    }
    LOGD("CuFailInternal() Finished!\n");
}

void CuFail_Line(CuTest *tc, const char *file, int line, const char *message2, const char *message)
{
    LOGD("CuFail_Line() Entered!\n");
    CuString string;
	if(!tc || !file || !message)
	{
		LOGE("Input Param is NULL!\n");
		return;
	}
    CuStringConstructor(&string);

    if (message2 != NULL)
    {
        CuStringAppend(&string, message2);
        CuStringAppend(&string, ": ");
    }

    CuStringAppend(&string, message);
    CuFailInternal(tc, file, line, &string);
	CuStringDestructor(&string);
    LOGD("CuFail_Line() Finished!\n");
}

void CuAssert_Line(CuTest *tc, const char *file, int line, const char *message, int condition)
{
    LOGD("CuAssert_Line() Entered!\n");
	if(!tc || !file || !message)
	{
		LOGE("Input Param is NULL!\n");
		return;
	}
    if (condition) 
	{
		return;
	}

    CuFail_Line(tc, file, line, NULL, message);
    LOGD("CuAssert_Line() Finished!\n");
}

void CuAssertStrEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                               const char *expected, const char *actual)
{
    LOGD("CuAssertStrEquals_LineMsg() Entered!\n");
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
    CuFailInternal(tc, file, line, &string);
    LOGD("CuAssertStrEquals_LineMsg() Finished!\n");
}

void CuAssertIntEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                               int expected, int actual)
{
    LOGD("CuAssertIntEquals_LineMsg() Entered!\n");
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
    CuFail_Line(tc, file, line, message, buf);
    LOGD("CuAssertIntEquals_LineMsg() Finished!\n");
}

void CuAssertDblEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                               double expected, double actual, double delta)
{
    LOGD("CuAssertDblEquals_LineMsg() Entered!\n");
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
    CuFail_Line(tc, file, line, message, buf);
    LOGD("CuAssertDblEquals_LineMsg() Finished!\n");
}

void CuAssertPtrEquals_LineMsg(CuTest *tc, const char *file, int line, const char *message,
                               void *expected, void *actual)
{
    LOGD("CuAssertPtrEquals_LineMsg() Entered!\n");
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

    sprintf(buf, "expected pointer <0x%p> but was <0x%p>", expected, actual);
    CuFail_Line(tc, file, line, message, buf);
    LOGD("CuAssertPtrEquals_LineMsg() Finished!\n");
}


/*-------------------------------------------------------------------------*
 * CuSuite
 *-------------------------------------------------------------------------*/

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

CuSuite *CuSuiteNew(void)
{
    LOGD("CuSuiteNew() Entered!\n");
    CuSuite *testSuite = CU_ALLOC(CuSuite);
    CuSuiteConstructor(testSuite);
    LOGD("CuSuiteNew() Finished!\n");
    return testSuite;
}

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
			testSuite->list[n]=NULL;
        }
    }
	testSuite->count = 0;
    testSuite->failCount = 0;
    LOGD("CuSuiteDestructor() Finished!\n");
}
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
//in this program ,
//if A is added with B,and A&B is the same type, 
//then B is robbed by A
//and B lost B's buffer loaded
//it's caused by the diff of shallow/deep copy, which can be solved by ref count
//generally,we don't use deep copy under the consideration of performance, geeks konw
void CuSuiteRobSuite(CuSuite *testSuite, CuSuite *testSuite2)
{
    LOGD("CuSuiteRobSuite() Entered!\n");
    int i;
	if(!testSuite || !testSuite2)
	{
		LOGE("Input Param is NULL!\n");
		return;
	}

    for (i = 0 ; i < testSuite2->count ; ++i)
    {
        CuTest *testCase = testSuite2->list[i];
        CuSuiteShallowAdd(testSuite, testCase);
    }
	//testSuite2->list is robbed, to avoid testSuite2->list[idx] be freeed twice
	testSuite2->count = 0;
    LOGD("CuSuiteRobSuite() Finished!\n");
}

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
