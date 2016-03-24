#include <assert.h>
#include <setjmp.h>
#include <string.h>

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
/*-------------------------------------------------------------------------*
 * Helper functions
 *-------------------------------------------------------------------------*/

#define CompareAsserts(tc, message, expected, actual)  X_CompareAsserts((tc), __FILE__, __LINE__, (message), (expected), (actual))

static void X_CompareAsserts(CuTest* tc, const char *file, int line, const char* message, const char* expected, const char* actual)
{
    LOGD("X_CompareAsserts() Entered!\n");
	int mismatch;
	if (expected == NULL || actual == NULL) {
		mismatch = (expected != NULL || actual != NULL);
	} else {
		const char *front = __FILE__ ":";
		const size_t frontLen = strlen(front);
		const size_t expectedLen = strlen(expected);

		const char *matchStr = actual;

		mismatch = (strncmp(matchStr, front, frontLen) != 0);
		if (!mismatch) {
			matchStr = strchr(matchStr + frontLen, ':');
			mismatch |= (matchStr == NULL || strncmp(matchStr, ": ", 2));
			if (!mismatch) {
				matchStr += 2;
				mismatch |= (strncmp(matchStr, expected, expectedLen) != 0);
			}
		}
	}

	CuTestAssert_Line(tc, file, line, message, !mismatch);
}

/*-------------------------------------------------------------------------*
 * CuString Test
 *-------------------------------------------------------------------------*/

void TestCuStringNew(CuTest* tc)
{
    LOGD("TestCuStringNew() Entered!\n");
	CuString* str = CuStringNew();
	CuAssertTrue(tc, 0 == str->length);
	CuAssertTrue(tc, 0 != str->size);
	CuAssertStrEquals(tc, "", str->buffer);
	
	CuStringDelete(str);
    LOGD("TestCuStringNew() Finished!\n");
}


void TestCuStringAppend(CuTest* tc)
{
    LOGD("TestCuStringAppend() Entered!\n");
	CuString* str = CuStringNew();
	CuStringAppend(str, "hello");
	CuAssertIntEquals(tc, 5, str->length);
	CuAssertStrEquals(tc, "hello", str->buffer);
	CuStringAppend(str, " world");
	CuAssertIntEquals(tc, 11, str->length);
	CuAssertStrEquals(tc, "hello world", str->buffer);
	
	CuStringDelete(str);
    LOGD("TestCuStringAppend() Finished!\n");
}


void TestCuStringAppendNULL(CuTest* tc)
{
    LOGD("TestCuStringAppendNULL() Entered!\n");
	CuString* str = CuStringNew();
	CuStringAppend(str, NULL);
	CuAssertIntEquals(tc, 4, str->length);
	CuAssertStrEquals(tc, "NULL", str->buffer);
	
	CuStringDelete(str);
    LOGD("TestCuStringAppendNULL() Finished!\n");
}


void TestCuStringAppendChar(CuTest* tc)
{
    LOGD("TestCuStringAppendChar() Entered!\n");
	CuString* str = CuStringNew();
	CuStringAppendChar(str, 'a');
	CuStringAppendChar(str, 'b');
	CuStringAppendChar(str, 'c');
	CuStringAppendChar(str, 'd');
	CuAssertIntEquals(tc, 4, str->length);
	CuAssertStrEquals(tc, "abcd", str->buffer);

	CuStringDelete(str);
    LOGD("TestCuStringAppendChar() Finished!\n");
}


void TestCuStringInserts(CuTest* tc)
{
    LOGD("TestCuStringInserts() Entered!\n");
	CuString* str = CuStringNew();
	CuStringAppend(str, "world");
	CuAssertIntEquals(tc, 5, str->length);
	CuAssertStrEquals(tc, "world", str->buffer);
	CuStringInsert(str, "hell", 0);
	CuAssertIntEquals(tc, 9, str->length);
	CuAssertStrEquals(tc, "hellworld", str->buffer);
	CuStringInsert(str, "o ", 4);
	CuAssertIntEquals(tc, 11, str->length);
	CuAssertStrEquals(tc, "hello world", str->buffer);
	CuStringInsert(str, "!", 11);
	CuAssertIntEquals(tc, 12, str->length);
	CuAssertStrEquals(tc, "hello world!", str->buffer);
	
	CuStringDelete(str);
    LOGD("TestCuStringInserts() Finished!\n");
}


void TestCuStringResizes(CuTest* tc)
{
    LOGD("TestCuStringResizes() Entered!\n");
	CuString* str = CuStringNew();
	int i;
	for(i = 0 ; i < STRING_MAX ; ++i)
	{
		CuStringAppend(str, "aa");
	}
	CuAssertTrue(tc, STRING_MAX * 2 == str->length);
	CuAssertTrue(tc, STRING_MAX * 2 <= str->size);
	CuStringDelete(str);
    LOGD("TestCuStringResizes() Finished!\n");
}

CuSuite* CuStringGetSuite(void)
{
    LOGD("CuStringGetSuite() Entered!\n");
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, TestCuStringNew);
	SUITE_ADD_TEST(suite, TestCuStringAppend);
	SUITE_ADD_TEST(suite, TestCuStringAppendNULL);
	SUITE_ADD_TEST(suite, TestCuStringAppendChar);
	SUITE_ADD_TEST(suite, TestCuStringInserts);
	SUITE_ADD_TEST(suite, TestCuStringResizes);

    LOGD("CuStringGetSuite() Finished!\n");
	return suite;
}

/*-------------------------------------------------------------------------*
 * CuTest Test
 *-------------------------------------------------------------------------*/

void TestPasses(CuTest* tc)
{
    LOGD("TestPasses() Entered!\n");
	CuAssert(tc, "test should pass", 1 == 0 + 1);
    LOGD("TestPasses() Finished!\n");
}

void zTestFails(CuTest* tc)
{
    LOGD("zTestFails() Entered!\n");
	CuAssert(tc, "test should fail", 1 == 1 + 1);
    LOGD("zTestFails() Finished!\n");
}


void TestCuTestNew(CuTest* tc)
{
    LOGD("TestCuTestNew() Entered!\n");
	CuTest* tc2 = CuTestNew("MyTest", TestPasses);
	CuAssertStrEquals(tc, "MyTest", tc2->name);
	CuAssertTrue(tc, !tc2->failed);
	CuAssertTrue(tc, tc2->message == NULL);
	CuAssertTrue(tc, tc2->function == TestPasses);
	CuAssertTrue(tc, tc2->ran == 0);
	CuAssertTrue(tc, tc2->jumpBuf == NULL);

	CuTestDelete(tc2);
    LOGD("TestCuTestNew() Finished!\n");
}


void TestCuTestConstructor(CuTest *tc)
{
    LOGD("TestCuTestConstructor() Entered!\n");
	CuTest tc2;
	CuTestConstructor(&tc2, "MyTest", TestPasses);
	CuAssertStrEquals(tc, "MyTest", tc2.name);
	CuAssertTrue(tc, !tc2.failed);
	CuAssertTrue(tc, tc2.message == NULL);
	CuAssertTrue(tc, tc2.function == TestPasses);
	CuAssertTrue(tc, tc2.ran == 0);
	CuAssertTrue(tc, tc2.jumpBuf == NULL);
	
	CuTestDestructor(&tc2);
    LOGD("TestCuTestConstructor() Finished!\n");
}

void TestCuAssert(CuTest* tc)
{
    LOGD("TestCuAssert() Entered!\n");
	CuTest tc2;
	CuTestConstructor(&tc2, "MyTest", TestPasses);

	CuAssert(&tc2, "test 1", 5 == 4 + 1);
	CuAssertTrue(tc, !tc2.failed);
	CuAssertTrue(tc, tc2.message == NULL);

	CuAssert(&tc2, "test 2", 0);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssert didn't fail", "test 2", tc2.message);

	CuAssert(&tc2, "test 3", 1);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssert didn't fail", "test 2", tc2.message);

	CuAssert(&tc2, "test 4", 0);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssert didn't fail", "test 4", tc2.message);
	
	CuTestDestructor(&tc2);
    LOGD("TestCuAssert() Finished!\n");

}

void TestCuAssertPtrEquals_Success(CuTest* tc)
{
    LOGD("TestCuAssertPtrEquals_Success() Entered!\n");
	CuTest tc2;
	int x;

	CuTestConstructor(&tc2, "MyTest", TestPasses);

	/* test success case */
	CuAssertPtrEquals(&tc2, &x, &x);
	CuAssertTrue(tc, ! tc2.failed);
	CuAssertTrue(tc, NULL == tc2.message);
		
	CuTestDestructor(&tc2);
    LOGD("TestCuAssertPtrEquals_Success() Finished!\n");

}

void TestCuAssertPtrEquals_Failure(CuTest* tc)
{
    LOGD("TestCuAssertPtrEquals_Failure() Entered!\n");
	CuTest tc2;
	int x;
	int* nullPtr = NULL;
	char expected_message[STRING_MAX];

	CuTestConstructor(&tc2, "MyTest", TestPasses);

	/* test failing case */
	sprintf(expected_message, "expected pointer <0x%p> but was <0x%p>", nullPtr, &x);
	CuAssertPtrEquals(&tc2, NULL, &x);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssertPtrEquals failed", expected_message, tc2.message);
		
	CuTestDestructor(&tc2);
    LOGD("TestCuAssertPtrEquals_Failure() Finished!\n");

}

void TestCuAssertPtrNotNull_Success(CuTest* tc)
{
    LOGD("TestCuAssertPtrNotNull_Success() Entered!\n");
	CuTest tc2;
	int x;

	CuTestConstructor(&tc2, "MyTest", TestPasses);

	/* test success case */
	CuAssertPtrNotNull(&tc2, (int)&x);
	//CuAssertPtrNotNull(&tc2, NULL);
	CuAssertTrue(tc, ! tc2.failed);
	CuAssertTrue(tc, NULL == tc2.message);
		
	CuTestDestructor(&tc2);
    LOGD("TestCuAssertPtrNotNull_Success() Finished!\n");

}

void TestCuAssertPtrNotNull_Failure(CuTest* tc)
{
    LOGD("TestCuAssertPtrNotNull_Failure() Entered!\n");
	CuTest tc2;

	CuTestConstructor(&tc2, "MyTest", TestPasses);

	/* test failing case */
	CuAssertPtrNotNull(&tc2, NULL);
	CuAssertTrue(tc, tc2.failed);
	CompareAsserts(tc, "CuAssertPtrNotNull failed", "null pointer unexpected", tc2.message);
		
	CuTestDestructor(&tc2);
    LOGD("TestCuAssertPtrNotNull_Failure() Finished!\n");

}

void TestCuTestRun(CuTest* tc)
{
    LOGD("TestCuTestRun() Entered!\n");
	CuTest tc2;
	CuTestConstructor(&tc2, "MyTest", zTestFails);
	CuTestRun(&tc2);

	CuAssertStrEquals(tc, "MyTest", tc2.name);
	CuAssertTrue(tc, tc2.failed);
	CuAssertTrue(tc, tc2.ran);
	CompareAsserts(tc, "TestRun failed", "test should fail", tc2.message);
		
	CuTestDestructor(&tc2);
    LOGD("TestCuTestRun() Finished!\n");

}

/*-------------------------------------------------------------------------*
 * CuSuite Test
 *-------------------------------------------------------------------------*/

void TestCuSuiteConstructor(CuTest* tc)
{
    LOGD("TestCuSuiteConstructor() Entered!\n");
	CuSuite ts;
	CuSuiteConstructor(&ts);
	CuAssertTrue(tc, ts.count == 0);
	CuAssertTrue(tc, ts.failCount == 0);
	
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteConstructor() Finished!\n");
}

void TestCuSuiteNew(CuTest* tc)
{
    LOGD("TestCuSuiteNew() Entered!\n");
	CuSuite* ts = CuSuiteNew();
	CuAssertTrue(tc, ts->count == 0);
	CuAssertTrue(tc, ts->failCount == 0);
	
	CuSuiteDelete(ts);
    LOGD("TestCuSuiteNew() Finished!\n");
}

void TestCuSuiteShallowAddTest(CuTest* tc)
{
    LOGD("TestCuSuiteShallowAddTest() Entered!\n");
	CuSuite ts;
	CuTest *tc2;

	CuSuiteConstructor(&ts);
	tc2 = CuTestNew("MyTest", zTestFails);

	CuSuiteShallowAdd(&ts, tc2);
	CuAssertTrue(tc, ts.count == 1);

	CuAssertStrEquals(tc, "MyTest", ts.list[0]->name);
	
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteShallowAddTest() Finished!\n");
}

void TestCuSuiteRobSuite(CuTest* tc)
{
    LOGD("TestCuSuiteRobSuite() Entered!\n");
	CuSuite* ts1 = CuSuiteNew();
	CuSuite* ts2 = CuSuiteNew();

	CuSuiteShallowAdd(ts1, CuTestNew("TestFails1", zTestFails));
	CuSuiteShallowAdd(ts1, CuTestNew("TestFails2", zTestFails));

	CuSuiteShallowAdd(ts2, CuTestNew("TestFails3", zTestFails));
	CuSuiteShallowAdd(ts2, CuTestNew("TestFails4", zTestFails));

	CuSuiteRobSuite(ts1, ts2);
	CuAssertIntEquals(tc, 4, ts1->count);

	CuAssertStrEquals(tc, "TestFails1", ts1->list[0]->name);
	CuAssertStrEquals(tc, "TestFails2", ts1->list[1]->name);
	CuAssertStrEquals(tc, "TestFails3", ts1->list[2]->name);
	CuAssertStrEquals(tc, "TestFails4", ts1->list[3]->name);
	
	CuSuiteDelete(ts2);
	CuSuiteDelete(ts1);
    LOGD("TestCuSuiteRobSuite() Finished!\n");
}

void TestCuSuiteRun(CuTest* tc)
{
    LOGD("TestCuSuiteRun() Entered!\n");
	CuSuite ts;
	CuTest *tc1, *tc2, *tc3, *tc4;

	CuSuiteConstructor(&ts);
	tc1 = CuTestNew("TestPasses", TestPasses);
	tc2 = CuTestNew("TestPasses", TestPasses);
	tc3 = CuTestNew("TestFails",  zTestFails);
	tc4 = CuTestNew("TestFails",  zTestFails);

	CuSuiteShallowAdd(&ts, tc1);
	CuSuiteShallowAdd(&ts, tc2);
	CuSuiteShallowAdd(&ts, tc3);
	CuSuiteShallowAdd(&ts, tc4);
	CuAssertTrue(tc, ts.count == 4);

	CuSuiteRun(&ts);
	CuAssertTrue(tc, ts.count - ts.failCount == 2);
	CuAssertTrue(tc, ts.failCount == 2);
	
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteRun() Finished!\n");
}

void TestCuSuiteSummary(CuTest* tc)
{
    LOGD("TestCuSuiteSummary() Entered!\n");
	CuSuite ts;
	CuTest *tc1, *tc2;
	CuString summary;

	CuSuiteConstructor(&ts);
	tc1 = CuTestNew("TestPasses", TestPasses);
	tc2 = CuTestNew("TestFails",  zTestFails);
	CuStringConstructor(&summary);

	CuSuiteShallowAdd(&ts, tc1);
	CuSuiteShallowAdd(&ts, tc2);
	CuSuiteRun(&ts);

	CuSuiteSummary(&ts, &summary);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 1);
	CuAssertStrEquals(tc, ".F\n\n", summary.buffer);

	
	CuStringDestructor(&summary);
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteSummary() Finished!\n");
}


void TestCuSuiteDetails_SingleFail(CuTest* tc)
{
    LOGD("TestCuSuiteDetails_SingleFail() Entered!\n");
	CuSuite ts;
	CuTest *tc1, *tc2;
	CuString details;
	const char* front;
	const char* back;

	CuSuiteConstructor(&ts);
	tc1 = CuTestNew("TestPasses", TestPasses);
	tc2 = CuTestNew("TestFails",  zTestFails);
	CuStringConstructor(&details);

	CuSuiteShallowAdd(&ts, tc1);
	CuSuiteShallowAdd(&ts, tc2);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 1);

	front = "There was 1 failure:\n"
		"1) TestFails: ";
	back =  "test should fail\n"
		"\n!!!FAILURES!!!\n"
		"Runs: 2 Passes: 1 Fails: 1\n";

	CuAssertStrEquals(tc, back, details.buffer + strlen(details.buffer) - strlen(back));
	details.buffer[strlen(front)] = 0;
	CuAssertStrEquals(tc, front, details.buffer);
	
	CuStringDestructor(&details);
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteDetails_SingleFail() Finished!\n");
}


void TestCuSuiteDetails_SinglePass(CuTest* tc)
{
    LOGD("TestCuSuiteDetails_SinglePass() Entered!\n");
	CuSuite ts;
	CuTest *tc1;
	CuString details;
	const char* expected;

	CuSuiteConstructor(&ts);
	tc1 = CuTestNew("TestPasses", TestPasses);
	CuStringConstructor(&details);

	CuSuiteShallowAdd(&ts, tc1);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 1);
	CuAssertTrue(tc, ts.failCount == 0);

	expected =
		"OK (1 test)\n";

	CuAssertStrEquals(tc, expected, details.buffer);

	CuStringDestructor(&details);
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteDetails_SinglePass() Finished!\n");
}

void TestCuSuiteDetails_MultiplePasses(CuTest* tc)
{
    LOGD("TestCuSuiteDetails_MultiplePasses() Entered!\n");
	CuSuite ts;
	CuTest *tc1, *tc2;
	CuString details;
	const char* expected;

	CuSuiteConstructor(&ts);
	tc1 = CuTestNew("TestPasses", TestPasses);
	tc2 = CuTestNew("TestPasses", TestPasses);
	CuStringConstructor(&details);

	CuSuiteShallowAdd(&ts, tc1);
	CuSuiteShallowAdd(&ts, tc2);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 0);

	expected =
		"OK (2 tests)\n";

	CuAssertStrEquals(tc, expected, details.buffer);
	CuStringDestructor(&details);
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteDetails_MultiplePasses() Finished!\n");
}

void TestCuSuiteDetails_MultipleFails(CuTest* tc)
{
    LOGD("TestCuSuiteDetails_MultipleFails() Entered!\n");
	CuSuite ts;
	CuTest *tc1, *tc2;
	CuString details;
	const char* front;
	const char* mid;
	const char* back;

	CuSuiteConstructor(&ts);
	tc1 = CuTestNew("TestFails1", zTestFails);
	tc2 = CuTestNew("TestFails2", zTestFails);
	CuStringConstructor(&details);

	CuSuiteShallowAdd(&ts, tc1);
	CuSuiteShallowAdd(&ts, tc2);
	CuSuiteRun(&ts);

	CuSuiteDetails(&ts, &details);

	CuAssertTrue(tc, ts.count == 2);
	CuAssertTrue(tc, ts.failCount == 2);

	front =
		"There were 2 failures:\n"
		"1) TestFails1: ";
	mid =   "test should fail\n"
		"2) TestFails2: ";
	back =  "test should fail\n"
		"\n!!!FAILURES!!!\n"
		"Runs: 2 Passes: 0 Fails: 2\n";

	CuAssertStrEquals(tc, back, details.buffer + strlen(details.buffer) - strlen(back));
	CuAssert(tc, "Couldn't find middle", strstr(details.buffer, mid) != NULL);
	details.buffer[strlen(front)] = 0;
	CuAssertStrEquals(tc, front, details.buffer);
	CuStringDestructor(&details);
	CuSuiteDestructor(&ts);
    LOGD("TestCuSuiteDetails_MultipleFails() Finished!\n");
}


/*-------------------------------------------------------------------------*
 * Misc Test
 *-------------------------------------------------------------------------*/

void TestCuStrDeepCopy(CuTest* tc)
{
    LOGD("TestCuStrDeepCopy() Entered!\n");
	const char* old = "hello world";
	const char* newStr = CuStrDeepCopy(old);
	CuAssert(tc, "old is new", strcmp(old, newStr) == 0);
	
	CuStrFree(newStr);
    LOGD("TestCuStrDeepCopy() Finished!\n");
}


void TestCuStringAppendFormat(CuTest* tc)
{
    LOGD("TestCuStringAppendFormat() Entered!\n");
	int i;
	char* text = CuStrAlloc(301);		/* long string */
	CuString* str = CuStringNew();
	for (i = 0 ; i < 300 ; ++i)
		text[i] = 'a';
	text[300] = '\0';
	CuStringAppendFormat(str, "%s", text);

	/* buffer limit raised to HUGE_STRING_LEN so no overflow */

	CuAssert(tc, "length of str->buffer is 300", 300 == strlen(str->buffer));

	CuStringDelete(str);

	CuStrFree(text);
    LOGD("TestCuStringAppendFormat() Finished!\n");
}

void TestFail(CuTest* tc)
{
    LOGD("TestFail() Entered!\n");
	jmp_buf buf;
	int pointReached = 0;
	CuTest* tc2 = CuTestNew("TestFails", zTestFails);
	tc2->jumpBuf = &buf;
	//如果是从setjmp直接调用返回，setjmp返回值为0。
	//如果是从longjmp恢复的程序调用环境返回，setjmp返回非零值
	if (setjmp(buf) == 0)
	{
		CuFail(tc2, "hello world");
		pointReached = 1;
	}
	CuAssert(tc, "point was not reached", pointReached == 0);
	CuTestDelete(tc2);
    LOGD("TestFail() Finished!\n");
}

void TestAssertStrEquals(CuTest* tc)
{
    LOGD("TestAssertStrEquals() Entered!\n");
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals", zTestFails);

	const char* expected = "expected <hello> but was <world>";
	const char *expectedMsg = "some text: expected <hello> but was <world>";

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, "hello", "world");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", "hello", "world");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals failed", expectedMsg, tc2->message);
	CuTestDelete(tc2);
    LOGD("TestAssertStrEquals() Finished!\n");
}

void TestAssertStrEquals_NULL(CuTest* tc)
{
    LOGD("TestAssertStrEquals_NULL() Entered!\n");
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals_NULL", zTestFails);

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, NULL, NULL);
	}
	CuAssertTrue(tc, !tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_NULL failed", NULL, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", NULL, NULL);
	}
	CuAssertTrue(tc, !tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_NULL failed", NULL, tc2->message);
	CuTestDelete(tc2);
    LOGD("TestAssertStrEquals_NULL() Finished!\n");
}

void TestAssertStrEquals_FailNULLStr(CuTest* tc)
{
    LOGD("TestAssertStrEquals_FailNULLStr() Entered!\n");
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals_FailNULLStr", zTestFails);

	const char* expected = "expected <hello> but was <NULL>";
	const char *expectedMsg = "some text: expected <hello> but was <NULL>";

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, "hello", NULL);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailNULLStr failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", "hello", NULL);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailNULLStr failed", expectedMsg, tc2->message);
	CuTestDelete(tc2);
    LOGD("TestAssertStrEquals_FailNULLStr() Finished!\n");
}

void TestAssertStrEquals_FailStrNULL(CuTest* tc)
{
    LOGD("TestAssertStrEquals_FailStrNULL() Entered!\n");
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertStrEquals_FailStrNULL", zTestFails);

	const char* expected = "expected <NULL> but was <hello>";
	const char *expectedMsg = "some text: expected <NULL> but was <hello>";

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals(tc2, NULL, "hello");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailStrNULL failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertStrEquals_Msg(tc2, "some text", NULL, "hello");
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals_FailStrNULL failed", expectedMsg, tc2->message);
	CuTestDelete(tc2);
    LOGD("TestAssertStrEquals_FailStrNULL() Finished!\n");
}

void TestAssertIntEquals(CuTest* tc)
{
    LOGD("TestAssertIntEquals() Entered!\n");
	jmp_buf buf;
	CuTest *tc2 = CuTestNew("TestAssertIntEquals", zTestFails);
	const char* expected = "expected <42> but was <32>";
	const char* expectedMsg = "some text: expected <42> but was <32>";
	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertIntEquals(tc2, 42, 32);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertIntEquals failed", expected, tc2->message);
	if (setjmp(buf) == 0)
	{
		CuAssertIntEquals_Msg(tc2, "some text", 42, 32);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertStrEquals failed", expectedMsg, tc2->message);
	CuTestDelete(tc2);
    LOGD("TestAssertIntEquals() Finished!\n");
}

void TestAssertDblEquals(CuTest* tc)
{
    LOGD("TestAssertDblEquals() Entered!\n");
	jmp_buf buf;
	double x = 3.33;
	double y = 10.0 / 3.0;
	CuTest *tc2 = CuTestNew("TestAssertDblEquals", zTestFails);
	char expected[STRING_MAX];
	char expectedMsg[STRING_MAX];
	sprintf(expected, "expected <%lf> but was <%lf>", x, y);
	sprintf(expectedMsg, "some text: expected <%lf> but was <%lf>", x, y);

	CuTestConstructor(tc2, "TestAssertDblEquals", TestPasses);

	CuAssertDblEquals(tc2, x, x, 0.0);
	CuAssertTrue(tc, ! tc2->failed);
	CuAssertTrue(tc, NULL == tc2->message);

	CuAssertDblEquals(tc2, x, y, 0.01);
	CuAssertTrue(tc, ! tc2->failed);
	CuAssertTrue(tc, NULL == tc2->message);

	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertDblEquals(tc2, x, y, 0.001);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertDblEquals failed", expected, tc2->message);
	tc2->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		CuAssertDblEquals_Msg(tc2, "some text", x, y, 0.001);
	}
	CuAssertTrue(tc, tc2->failed);
	CompareAsserts(tc, "CuAssertDblEquals failed", expectedMsg, tc2->message);
	CuTestDelete(tc2);
    LOGD("TestAssertDblEquals() Finished!\n");
}

/*-------------------------------------------------------------------------*
 * main
 *-------------------------------------------------------------------------*/

CuSuite* CuGetSuite(void)
{
    LOGD("CuGetSuite() Entered!\n");
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestCuStringAppendFormat);

	SUITE_ADD_TEST(suite, TestCuStrDeepCopy);
	SUITE_ADD_TEST(suite, TestFail);
	SUITE_ADD_TEST(suite, TestAssertStrEquals);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_NULL);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_FailStrNULL);
	SUITE_ADD_TEST(suite, TestAssertStrEquals_FailNULLStr);
	SUITE_ADD_TEST(suite, TestAssertIntEquals);
	SUITE_ADD_TEST(suite, TestAssertDblEquals);

	SUITE_ADD_TEST(suite, TestCuTestNew);
	SUITE_ADD_TEST(suite, TestCuTestConstructor);
	SUITE_ADD_TEST(suite, TestCuAssert);
	SUITE_ADD_TEST(suite, TestCuAssertPtrEquals_Success);
	SUITE_ADD_TEST(suite, TestCuAssertPtrEquals_Failure);
	SUITE_ADD_TEST(suite, TestCuAssertPtrNotNull_Success);
	SUITE_ADD_TEST(suite, TestCuAssertPtrNotNull_Failure);
	SUITE_ADD_TEST(suite, TestCuTestRun);

	SUITE_ADD_TEST(suite, TestCuSuiteConstructor);
	SUITE_ADD_TEST(suite, TestCuSuiteNew);
	SUITE_ADD_TEST(suite, TestCuSuiteShallowAddTest);
	SUITE_ADD_TEST(suite, TestCuSuiteRobSuite);
	SUITE_ADD_TEST(suite, TestCuSuiteRun);
	SUITE_ADD_TEST(suite, TestCuSuiteSummary);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_SingleFail);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_SinglePass);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_MultiplePasses);
	SUITE_ADD_TEST(suite, TestCuSuiteDetails_MultipleFails);

    LOGD("CuGetSuite() Finished!\n");
	return suite;
}
