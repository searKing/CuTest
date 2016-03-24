#ifndef CU_TEST_H
#define CU_TEST_H

#include <setjmp.h>
#include <stdarg.h>

#define CUTEST_VERSION  "CuTest 1.5"

/* CuString */

char* CuStrAllocBySize(int size);
char* CuStrReallocByStr(const char* old);

#define CU_ALLOC(TYPE)		((TYPE*) malloc(sizeof(TYPE)))

#define HUGE_STRING_LEN	8192
#define STRING_MAX		256
#define STRING_INC		256

typedef struct
{
	int length;//used size
	int size;//buffer total size
	char* buffer;
} CuString;

void CuStringConstructor(CuString* str);
CuString* CuStringNew(void);
void CuStringDestructor(CuString *str);
void CuStringDelete(CuString *str);
void CuStringRead(CuString* str, const char* path);
void CuStringAppend(CuString* str, const char* text);
void CuStringAppendChar(CuString* str, char ch);
void CuStringAppendFormat(CuString* str, const char* format, ...);
void CuStringInsert(CuString* str, const char* text, int pos);
void CuStringResize(CuString* str, int newSize);
void CuStringDestructor(CuString* str);

/* CuTest */

typedef struct CuTest CuTest;

typedef void (*TestFunction)(CuTest *);

struct CuTest
{
	char* name;	//本测试的名字--CuStrAlloc--
	TestFunction function;
	int failed;//本测试用例是否通过
	int ran;//设置setjmp完成
	const char* message;//错误原因，assert返回
	jmp_buf *jumpBuf;//for setjmp和longjmp, used as goto
};

void CuTestConstructor(CuTest* t, const char* name, TestFunction function);//--CuTestShallowDelete
CuTest* CuTestNew(const char* name, TestFunction function);//--CuTestDestructor
void CuTestRun(CuTest* tc);
void CuTestDestructor(CuTest *t);
void CuTestShallowDelete(CuTest *t);

/* Internal versions of assert functions -- use the public versions */
void CuTestFail_Line(CuTest* tc, const char* file, int line, const char* message2, const char* message);
void CuTestAssert_Line(CuTest* tc, const char* file, int line, const char* message, int condition);
void CuTestAssertStrEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	const char* expected, const char* actual);
void CuTestAssertIntEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	int expected, int actual);
void CuTestAssertDblEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	double expected, double actual, double delta);
void CuTestAssertPtrEquals_LineMsg(CuTest* tc, 
	const char* file, int line, const char* message, 
	void* expected, void* actual);

/* public assert functions */

#define CuFail(tc, ms)                        CuTestFail_Line(  (tc), __FILE__, __LINE__, NULL, (ms))
#define CuAssert(tc, ms, cond)                CuTestAssert_Line((tc), __FILE__, __LINE__, (ms), (cond))
#define CuAssertTrue(tc, cond)                CuTestAssert_Line((tc), __FILE__, __LINE__, "assert failed", (cond))

#define CuAssertStrEquals(tc,ex,ac)           CuTestAssertStrEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define CuAssertStrEquals_Msg(tc,ms,ex,ac)    CuTestAssertStrEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define CuAssertIntEquals(tc,ex,ac)           CuTestAssertIntEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define CuAssertIntEquals_Msg(tc,ms,ex,ac)    CuTestAssertIntEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))
#define CuAssertDblEquals(tc,ex,ac,dl)        CuTestAssertDblEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac),(dl))
#define CuAssertDblEquals_Msg(tc,ms,ex,ac,dl) CuTestAssertDblEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac),(dl))
#define CuAssertPtrEquals(tc,ex,ac)           CuTestAssertPtrEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
#define CuAssertPtrEquals_Msg(tc,ms,ex,ac)    CuTestAssertPtrEquals_LineMsg((tc),__FILE__,__LINE__,(ms),(ex),(ac))

#define CuAssertPtrNotNull(tc,p)        CuTestAssert_Line((tc),__FILE__,__LINE__,"null pointer unexpected",(p != NULL))
#define CuAssertPtrNotNullMsg(tc,msg,p) CuTestAssert_Line((tc),__FILE__,__LINE__,(msg),(p != NULL))

/* CuSuite */

#define MAX_TEST_CASES	1024

#define SUITE_ADD_TEST(SUITE,TEST)	CuSuiteShallowAdd(SUITE, CuTestNew(#TEST, TEST))
typedef struct
{
	int count;//输入测试用例的个数
	CuTest* list[MAX_TEST_CASES];//测试用例
	int failCount;//错误的测试用例个数
}CuSuite;


void CuSuiteConstructor(CuSuite* testSuite);
CuSuite* CuSuiteNew(void);
void CuSuiteDestructor(CuSuite *testSuite);
void CuSuiteDelete(CuSuite *testSuite);
void CuSuiteShallowAdd(CuSuite* testSuite, CuTest *testCase);
void CuSuiteRobSuite(CuSuite* testSuite, CuSuite* testSuite2);
void CuSuiteRun(CuSuite* testSuite);
void CuSuiteSummary(CuSuite* testSuite, CuString* summary);
void CuSuiteDetails(CuSuite* testSuite, CuString* details);

#endif /* CU_TEST_H */
