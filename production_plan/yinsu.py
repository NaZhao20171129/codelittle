#! /usr/bin/python
#coding=utf-8
import xlrd
import xlwt
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
from pylab import *



reload(sys)
sys.setdefaultencoding('utf-8')

PART_KE_YUZHI=6
PART_NUM=12
SUO_NUM=4
SHEET_NUM=4
COL_NUM=10
LAST=43


filename='附件1-楚雄供电局生产计划及执行情况统计（2010,2011,2012,2013）.xls'

plt.figure(1)
plt.figure(2)
#plt.figure(3)
#plt.figure(4)


def getcontext(worksheet):
        #worksheet = workbook.sheet_by_name(worksheet_name)
        #遍历worksheet1中所有row
        num_rows = worksheet.nrows
        curr_row = 0
        #while curr_row < num_rows:
        #    row = worksheet1.row_values(curr_row)
        #    print('row%s is %s' %(curr_row,row))
        #    curr_row += 1


	#遍历worksheet1中所有col
        num_cols = worksheet.ncols
        curr_col = 0
        #while curr_col < num_cols:
        #    col = worksheet1.col_values(curr_col)
        #    print('col%s is %s' %(curr_col,col))
        #    curr_col += 1
            
        #遍历worksheet1中所有cell
        context=[]
        onerow=[]

        for rown in range(num_rows):
            onerow=[]
            for coln in range(num_cols):
                cell = worksheet.cell_value(rown,coln)
                if cell==u'无' or not cell:
                    cell=0
                onerow.append(cell)
            context.append(onerow)
            #print onerow
        #print context
        print context[9][10]
        return context


def readcontent(filename):
        workbook = xlrd.open_workbook(filename)
        #在workbook（工作簿）中抓取所有sheet的名称
        worksheets = workbook.sheet_names()

        #在workbook（工作簿）中根据名称抓取sheet1
        worksheet1 = workbook.sheet_by_name(u'2011年')
        """
        #通过索引顺序获取
        worksheet1 = workbook.sheets()[0]
        print worksheet1
        #或
        worksheet1 = workbook.sheet_by_index(0)
        print worksheet1
        """

        fileout=xlwt.Workbook()
        #在worksheet中遍历所有sheet
        count=0
        for worksheet_name in worksheets:
            print 'c=%d'%count
            plt.figure(count)
            worksheet = workbook.sheet_by_name(worksheet_name)
            c=getcontext(worksheet)

            table=fileout.add_sheet(worksheet_name,cell_overwrite_ok=True)
	    year=worksheet_name


            startline=7
	    
	    max=0
            for i in range(0,4):
                print 'i=%d' %i
                m=count_and_finished(c,startline,table,i,year)
                startline+=PART_NUM
		if m>max:
			 max=m

	    #plt.show()
	    ylim(0,max*1.1)
            plt.savefig("s"+str(count)+".png", format='png')
            count+=1
	    #break
        fileout.save('fuck.xls')
		    

                 
        """
        #其他方法
        cell = worksheet1.cell(rown,coln).value
        cell = worksheet1.row(rown)[coln].value
        cell = worksheet1.col(coln)[rown].value
        
        #获取单元格中值得类型，类型 0 empty,1 string, 2 number, 3 date, 4 boolean, 5 error
        cell_type = worksheet1.cell_type(rown,coln)
        print cell_type
"""


def count_and_finished(context,startline,table,number,year):
        suodui=context[startline][0][0:2]
        yuzhixing=context[startline][1]
        title_list=[]
        count_list=[]
        num_finished_list=[]
        money_finished_list=[]
	money_list=[]

        for row in range(startline,startline+PART_NUM):
			
                title=str(context[row][2])
                table.write(row,0,unicode(title,'utf-8'))
		title_list.append(unicode(title,'utf-8'))

                count_per=context[row][4]
                count_list.append(count_per)

                money_per=context[row][6]
                money_list.append(money_per)


	if(startline==LAST):
		print 'yes'
	x=[]
	for j in range(0,len(count_list)):
		cur=j+number*0.2
		x.append(cur)
	font = FontProperties(fname=r"/usr/share/fonts/truetype/wqy/wqy-microhei.ttc", size=14) 
	

    
        if(number==0):
                col='r'
        elif (number==1):
                col='blue'
        elif (number==2):
                col='g'
        else:
             col='black'

	#plt.title(suodui,fontproperties=font)
	plt.title(year,fontproperties=font)
	first=plt.bar(x,count_list,align='center',color=col,label=suodui,width=0.20)
	xlim(-0.2,8.3)
	plt.legend(prop=font,loc="upper right")
#	first=plt.bar(x,count_list,align='center',label=suodui)
	#plt2=plt.twinx()

	#plt2.plot(x,num_finished_list,color='red')
	label_font = FontProperties(fname=r"/usr/share/fonts/truetype/wqy/wqy-microhei.ttc", size=10) 
	if(number==1):
		plt.xticks(x,title_list,fontproperties=label_font,rotation=25)

	for a,b in zip(x,count_list):
		if(b<0.001):
			text(a, b+0.05, '0' , ha='center', va= 'bottom',size=6)
		else:
			text(a, b+0.05, '%1.0f%%' % (b*100), ha='center', va= 'bottom',size=6)




	return max(count_list)


def main():
	readcontent(filename)
 
if __name__=='__main__':
	main()
