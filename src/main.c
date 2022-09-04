/****************************************************************
* Copyright © zuozhongkai Co., Ltd. 1998-2018. All rights reserved.
* File name: 
* Author: 
* Version:
* Description:
* Others:
* Log: 
*****************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "parseh264.h"
#include "rgb2yuv.h"

/*
	this is a great dream
*/

/*
*@Description: 函数描述，描述本函数的基本功能
*@param 1– 参数1. 
*@param 2– 参数2 
*@return – 返回值 
*/
int main(int argc,char *argv[])
{
	char *path = "../src13.h264";
	long max_nalu = 100000;

	simplest_h264_parser(path, max_nalu);

	return 0;
}



