#include	"show_picture.h"


static UINT tjd_input(JDEC* jd,BYTE* buff,UINT nd);
static UINT tjd_output(JDEC* jd,void* bitmap,JRECT* rect);


//JPG
//�������ص�����
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
//˵�������Ҫ�ı�ͼƬ������λ������Ҫ��Ӧ�ı�
//TFT_SetWindow ������ i��ѭ��ֵ���ͽ���������Ӧ
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

//FileName:Ҫ��ʾ��ͼƬ�ļ�  JPG
//file_path:·��
//ͼƬ��ʽ 400X240
//����0������ɹ�  1������ʧ��	2:���ļ�ʧ��
//��JD_SZBUF�����ʱ��workbuf[]ҲҪ��Ӧ����
//u8	workbuf[3100]; 
int Load_JPG_File(const TCHAR* file_path)
{
	JDEC 	jd;
	JRESULT rc;
	FIL file_jpg;
	int res;
	u32 size=4096;
	u8 *workbuf=NULL;
	workbuf = (u8*)mem_malloc(size);		//�����ڴ�
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
	mem_free(workbuf);			 //�ͷ��ڴ�
	workbuf=NULL;
	return 0;
			
}







