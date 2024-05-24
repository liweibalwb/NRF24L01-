#include	"show_picture.h"


static UINT tjd_input(JDEC* jd,BYTE* buff,UINT nd);
static UINT tjd_output(JDEC* jd,void* bitmap,JRECT* rect);


//JPG
//数据填充回调函数
static UINT tjd_input(JDEC* jd,BYTE* buff,UINT nd)
{
	UINT rb;
	FIL *fil=(FIL*)jd->device;

	if(buff)
	{
		f_read(fil, buff, nd, &rb);
		return rb;
	} 
	else return (f_lseek(fil,f_tell(fil)+nd)==FR_OK)?nd:0;
}

//JPG
//说明：如果要改变图片比例和位置则需要相应改变
//TFT_SetWindow 参数和 i的循环值，和解码比列相对应
//jd: Decompression object 
//bitmap:RGB bitmap to be output 
//rect:Rectangular region to output 
//Description:The size of rectangular varies from 1x1 to 16x16 
//depends on clipping, scaling and sampling factor of the image.
static UINT tjd_output(JDEC* jd,void* bitmap,JRECT* rect)
{
	u32 i=0,j=0;
	u16 *pcolor;
	jd = jd;
	pcolor=(u16*)bitmap;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
	 		TFT_DrawPoint(rect->left+j,rect->top+i,*pcolor++);
		}
	}
	return 1;	
}

//FileName:要显示的图片文件  JPG
//file_path:路径
//图片格式 400X240
//返回0：解码成功  1：解码失败	2:打开文件失败
//当JD_SZBUF增大的时候workbuf[]也要相应增大
//u8	workbuf[3100]; 
int Load_JPG_File(const TCHAR* file_path)
{
	JDEC 	jd;
	JRESULT rc;
	FIL file_jpg;
	int res;
	u32 size=4096;
	u8 *workbuf=NULL;
	workbuf = (u8*)mem_malloc(size);		//申请内存
	if(workbuf==NULL){
		return 1;
	}
	res=f_open(&file_jpg,file_path,FA_READ);
	if(res) return 2;
	rc=jd_prepare(&jd,tjd_input,workbuf,size,&file_jpg);
	rc=jd_decomp(&jd,tjd_output,0);
	f_close(&file_jpg);
	if(rc!=JDR_OK)
	{
		mem_free(workbuf);
		workbuf=NULL;
		return 1;
	}
	mem_free(workbuf);			 //释放内存
	workbuf=NULL;
	return 0;
			
}







