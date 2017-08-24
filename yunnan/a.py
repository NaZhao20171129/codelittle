#! /usr/bin/python
#coding=utf-8
import xlrd
import xlwt
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

PART_KE_YUZHI=6
PART_NUM=12
SUO_NUM=4
SHEET_NUM=4
COL_NUM=10


filename='附件1-楚雄供电局生产计划及执行情况统计（2010,2011,2012,2013）.xls'
def readcontent(filename):
        #打开一个workbook:
        #workbook = xlrd.open_workbook('附件1-楚雄供电局生产计划及执行情况统计（2010,2011,2012,2013）.xls')
        workbook = xlrd.open_workbook(filename)
        #在workbook（工作簿）中抓取所有sheet的名称
        worksheets = workbook.sheet_names()
        print'worksheets is ' 
        for ws in worksheets:
                print ws

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

        #在worksheet中遍历所有sheet
        #for worksheet_name in worksheets:
        #    worksheet = workbook.sheet_by_name(worksheet_name)
        #    print('worksheet is %s' %worksheet)

        #遍历worksheet1中所有row
        num_rows = worksheet1.nrows
        print 'rownum=%d' %num_rows
        curr_row = 0
        #while curr_row < num_rows:
        #    row = worksheet1.row_values(curr_row)
        #    print('row%s is %s' %(curr_row,row))
        #    curr_row += 1
            
        #遍历worksheet1中所有col
        num_cols = worksheet1.ncols
        print 'colnum=%d' %num_cols
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
                cell = worksheet1.cell_value(rown,coln)
                if cell==u'无' or not cell:
                    cell=0
                onerow.append(cell)
            context.append(onerow)
            #print onerow
        #print context
        print context[9][10]
        return context
"""
        #其他方法
        cell = worksheet1.cell(rown,coln).value
        print cell
        #或
        cell = worksheet1.row(rown)[coln].value
        print cell
        #或
        cell = worksheet1.col(coln)[rown].value
        print cell
        
        #获取单元格中值得类型，类型 0 empty,1 string, 2 number, 3 date, 4 boolean, 5 error
        cell_type = worksheet1.cell_type(rown,coln)
        print cell_type
"""


def count_and_finished(context):
        fileout=xlwt.Workbook()
        table=fileout.add_sheet('what',cell_overwrite_ok=True)

        #title=[]
        #count=[]
        #finished_num=[]
        #finished_money=[]
        tlist=[]
        suodui=context[7][0]
        yuzhixing=context[7][1]
        for row in range(7,7+PART_NUM):
                title=str(suodui)+'_'+str(yuzhixing)+'_'+str(context[row][2])
                table.write(row,0,unicode(title,'utf-8'))


                count=context[row][3]
                print count
                table.write(row,1,count)

                finished_num=(float(context[row][5])*float(context[row][8])+float(context[row][4])*float(context[row][9]))
                table.write(row,2,finished_num)


                finished_money=(context[row][7]*context[row][8]+context[row][6]*context[row][9])
                table.write(row,3,finished_money)

        fileout.save('fuck.xls')




        
c=readcontent(filename)
print c[7][4]
print c[11][4]
count_and_finished(c)
