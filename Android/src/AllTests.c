
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
CuSuite* CuGetSuite();
CuSuite* CuStringGetSuite();

void RunAllTests(void)
{
    LOGD("RunAllTests() Entered!\n");
	CuString *output = CuStringNew();//输出结构体
	CuSuite* suite = CuSuiteNew();//测试组句柄
	CuSuite* suite_test = NULL;//测试组句柄

	//增加的测试组
	printf("Step1\n");
	suite_test = CuGetSuite();
	CuSuiteRobSuite(suite, suite_test);
	CuSuiteDelete(suite_test);
	printf("Step2\n");
	suite_test = CuStringGetSuite();
	CuSuiteRobSuite(suite, suite_test);
	CuSuiteDelete(suite_test);
	printf("Step3\n");
	CuSuiteRun(suite);
	printf("Step4\n");
	CuSuiteSummary(suite, output);
	printf("Step5\n");
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	
	CuSuiteDelete(suite);
	CuStringDelete(output);
}

int main(void)
{
    LOGD("main() Entered!\n");
	RunAllTests();
    LOGD("main() Finished!\n");
	return 0;
}


