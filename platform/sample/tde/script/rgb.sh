#!/bin/sh
sample=ak_tde_sample
pic_src=./anyka.logo.160.577.rgb
pic_bg=./desert.1024.600.rgb
format_bg=1
format_logo=3

$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --reset-screen --opt-colorkey FFFF00,FFFFFF,1
$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
while :
do
	#echo "����";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "����|͸����";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "����|��ɫ����";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "����|��ɫ����";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	#echo "����|��ɫ����|͸����";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7
	#echo "����|��ɫ����|͸����";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7

	#echo "��ת90"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "��ת90|͸����"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "��ת90|��ɫ����"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "��ת90|��ɫ����|͸����"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7

	#echo "��ת180"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "��ת180|͸����"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "��ת180|��ɫ����"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "��ת180|��ɫ����|͸����"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,432,12,160,577 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7

	#echo "��ת270"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "��ת270|͸����"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "��ת270|��ɫ����"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "��ת270|��ɫ����|͸����"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,224,220,577,160 --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7

	#echo "����"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s "160,577,362,0,300,600" --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "����|͸����"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s "160,577,362,0,300,600" --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "����|��ɫ����"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s "160,577,362,0,300,600" --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,1
	#echo "����|��ɫ����|͸����"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s "160,577,362,0,300,600" --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,1 --opt-transparent 7
	#echo "����|��ɫ����"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s "160,577,362,0,300,600" --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,0
	#echo "����|��ɫ����|͸����"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s "160,577,362,0,300,600" --rect-bg 1024,600 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,0 --opt-transparent 7
done