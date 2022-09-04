#include <stdio.h>
#include <stdlib.h>

#include "parseaac.h"

/*******************************************************************************************
 * AAC 音频编码标准: 分为流格式和文件格式
 * 
 * 文件格式: adif格式
 * 该格式特点：只有开头有一个头部信息，后面都是AAC裸数据。适应磁盘存储和文件播放
 * 流格式: adts_frame格式
 * 该格式特点：每一帧数据=固定头(fixed_header)+ 可变头(variable_header)+帧数据(raw_data)，适合流媒体在线播放。
 * 
 * ads_fixed_header  固定头占用28位
 * syncword;                   12bit   同步头,代表一个帧开始,总是0xfff
 * ID;                          1bit   0标识MPEG-4,1标识MPEG-2  
 * layer;                       2bit   always: '00'
 * protection_absent;           1bit   表示是否误码校验。1表示no CRC,1时adts头7字节,否则9字节
 * profile;                     2bit   表示使用哪个级别的AAC，如01 Low Complexity(LC)--- AAC LC
 * sampling_frequency_index;    4bit   表示使用的采样率下标,查数组得值
 * private_bit;                 1bit
 * channel_configuration;       3bit   表示声道数，比如2表示立体声双声道
 * original_copy;               1bit
 * home;                        1bit
 * 
 * adts_variable_header 可变头占用28位
 * copyright_identification_bit;        1bit
 * copyright_identification_start;      1bit
 * ac_frame _length;                   13bit  ADTS帧的总长度,包括ADTS头和AAC原始流
 * ads_buffer_fullness;                11bit  0x7FF说明是码率可变的码流
 * number_of_raw_data_blocks_in_frame;  2bit  表示ADTS帧中有+1个AAC原始帧
 * 
 * 一个原始帧包含1024个采样
 * 一个AAC音频帧的播放时间=一个AAC帧对应的采样样本的个数/采样率
 * 总时间t=总帧数x一个AAC音频帧的播放时间
 * 时间t=总帧数x一个AAC音频帧的播放时间
 * 
 *******************************************************************************************/

int samplingFrequencylndex[16] = {
    96000,
    88200,
    64000,
    48000,
    44100,
    32000,
    24000,
    22050,
    16000,
    12000,
    11025,
    8000,
    7350,
    0,      /* reserved */
    0,      /* reserved */
    0,      /* escape value */
};

static int FindSyncWord(unsigned char *Buf){
	if(Buf[0] != 0xff || (Buf[1] & 0xf0) != 0xf0 ) //0xfff?
        return 0; 
	else 
        return 1;
}

int GetADTSHeader(FILE *fp)
{
    int offset = 0;
    unsigned char buf[3] = {0};

    if(3 != fread(buf, 1, 2, fp))
        return -1;

    while(!feof(fp)){     
        if(!FindSyncWord(buf)){
            if(1 != fread(buf+2, 1, 1, fp))
                return -1;

            buf[0] = buf[1];
            buf[1] = buf[2];
        }
        else{
            break;
        }
    }

    offset = ftell(fp);

    return offset;
}

int GetADTSFrame(FILE *fp, unsigned char *buffer, long *offset)
{
    if(fp == NULL || buffer == NULL)
        return -1;

    *offset = GetADTSHeader(fp);
    if(*offset < 0)
        return -1;

    if(7 != fread(buffer, 1, 7, fp))
        return -1;

    int size = 0;

    size |= ((buffer[3] & 0x03) <<11);   //high 2 bit
	size |= buffer[4]<<3;                //middle 8 bit
	size |= ((buffer[5] & 0xe0)>>5);     //low 3bit

    return size;
}


int simplest_aac_parser(char *url)
{
    long offset;
    int  size, cnt = 0;

    unsigned char *aacframe=(unsigned char *)malloc(16);
	unsigned char *aacbuffer=(unsigned char *)malloc(1024*1024);
 
	FILE *ifile = fopen(url, "r+");
	if(!ifile){
		printf("Open file error");
		return -1;
	}
 
	printf("-----+- ADTS Frame Table -+------+\n");
	printf(" NUM | Profile | Frequency| Size |\n");
	printf("-----+---------+----------+------+\n");

    while(!feof(ifile)){
            size = GetADTSFrame(ifile, aacframe, &offset);
			if(size == -1){
				break;
			}
 
			char profile_str[10]={0};
			char frequence_str[10]={0};
 
			unsigned char profile=aacframe[2]&0xC0;
			profile=profile>>6;
			switch(profile){
			case 0: sprintf(profile_str,"Main");break;
			case 1: sprintf(profile_str,"LC");break;
			case 2: sprintf(profile_str,"SSR");break;
			default:sprintf(profile_str,"unknown");break;
			}
 
			unsigned char sampling_frequency_index=aacframe[2]&0x3C;
			sampling_frequency_index=sampling_frequency_index>>2;
			switch(sampling_frequency_index){
			case 0: sprintf(frequence_str,"96000Hz");break;
			case 1: sprintf(frequence_str,"88200Hz");break;
			case 2: sprintf(frequence_str,"64000Hz");break;
			case 3: sprintf(frequence_str,"48000Hz");break;
			case 4: sprintf(frequence_str,"44100Hz");break;
			case 5: sprintf(frequence_str,"32000Hz");break;
			case 6: sprintf(frequence_str,"24000Hz");break;
			case 7: sprintf(frequence_str,"22050Hz");break;
			case 8: sprintf(frequence_str,"16000Hz");break;
			case 9: sprintf(frequence_str,"12000Hz");break;
			case 10: sprintf(frequence_str,"11025Hz");break;
			case 11: sprintf(frequence_str,"8000Hz");break;
			default:sprintf(frequence_str,"unknown");break;
			}
 
            fseek(ifile, size, SEEK_CUR);

			fprintf(stdout, "%5d| %8s|  %8s| %5d|\n", cnt, profile_str, frequence_str, size);
			cnt++;  
    }

    fclose(ifile);
	free(aacbuffer);
	free(aacframe);

    return 0;
}