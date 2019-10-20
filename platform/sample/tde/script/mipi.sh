#!/bin/sh
sample=ak_tde_sample
pic_src=./anyka.logo.160.577.rgb
pic_bg=./desert.800.1280.rgb
format_bg=3
format_logo=3

$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --reset-screen --opt-colorkey FFFF00,FFFFFF,1
$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
while :
do
	#echo "拷贝";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "拷贝|透明度";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "拷贝|颜色过滤";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "拷贝|颜色过滤";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	#echo "拷贝|颜色过滤|透明度";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7
	#echo "拷贝|颜色过滤|透明度";sleep 1
	$sample --opt-blit --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7

	#echo "旋转90"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "旋转90|透明度"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "旋转90|颜色过滤"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "旋转90|颜色过滤|透明度"
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7
	$sample --opt-rotate 1 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7

	#echo "旋转180"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "旋转180|透明度"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "旋转180|颜色过滤"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "旋转180|颜色过滤|透明度"
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7
	$sample --opt-rotate 2 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,320,352,160,577 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7

	#echo "旋转270"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "旋转270|透明度"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "旋转270|颜色过滤"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0
	#echo "旋转270|颜色过滤|透明度"
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,1 --opt-transparent 7
	$sample --opt-rotate 3 --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,112,560,577,160 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey FFFF00,FFFFFF,0 --opt-transparent 7

	#echo "拉伸"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,250,250,300,800 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo
	#echo "拉伸|透明度"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,250,250,300,800 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-transparent 7
	#echo "拉伸|颜色过滤"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,250,250,300,800 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,1
	#echo "拉伸|颜色过滤|透明度"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,250,250,300,800 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,1 --opt-transparent 7
	#echo "拉伸|颜色过滤"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,250,250,300,800 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,0
	#echo "拉伸|颜色过滤|透明度"
	$sample --opt-scale --file-s $pic_src --file-bg $pic_bg --rect-s 160,577,250,250,300,800 --rect-bg 800,1280 --format-bg $format_bg --format-out 1 --format-logo $format_logo --opt-colorkey F00000,FFFFFF,0 --opt-transparent 7
done