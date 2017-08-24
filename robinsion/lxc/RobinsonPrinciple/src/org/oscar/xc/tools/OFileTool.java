/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.oscar.xc.tools;

import java.util.List;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.io.BufferedWriter;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

/**
 *
 * @author LXC
 */

public class OFileTool {
    
    /* 
     * 按起止标记读取文件，把文件内容存储到List中。
     * filePath为文件路径，segBegin为段起始标记，segEnd为段结束标记，segSepTag用来设定区分段中每行的标记
     */
    public static List<String> readFileSegments(String filePath, String segBegin, String segEnd, String segSepTag) {
        
        List<String> segList = new ArrayList<String>();
        
        File file = new File(filePath); // 读取文件      
        
        if(!file.exists() || !file.canRead()){	// 判断文件是否可读，存在
        	System.out.println("文件不存在或不可读");
        	return segList;
        }
        
        BufferedReader br = null;      
        try{
            br = new BufferedReader(new FileReader(file));
            
            String tmpStr = ""; // 临时存储文件的一行            
            String segStr = ""; // 存储文件的一段            
            Boolean addStrFlag = false; // 读取段落标记
            
            while((tmpStr=br.readLine()) != null)
            {
                if (tmpStr.startsWith(segBegin)) {  // 当行含有起始标记是开始存储文件内容                  
                	addStrFlag = true;
                }
                
                if (addStrFlag){    // 文件内容连续加入segStr中，用segSepTag标记分割开每一行
                    segStr = segStr + tmpStr + segSepTag;
                }
                
                if (tmpStr.startsWith(segEnd)) {    // 如果遇到段落结尾标记，把segStr加入List中，初始化标记                   segList.add(segStr);
                    segStr = "";
                    addStrFlag = false;
                }
            }   
            br.close();
            
        } catch (IOException e) {                   
            e.printStackTrace();
        }    
        
        return segList;
    }
    

    /*
     *   正常读文件，返回buffer
     * */
    public static BufferedReader generalReadFile(String filePath) {
    	
    	File file = new File(filePath);
    	BufferedReader br = null;  
    	
    	if(!file.exists() || !file.canRead()){	// 判断文件是否可读，存在
        	System.out.println("文件不存在或不可读");
        	return br;
        }
    	
        try{
            br = new BufferedReader(new FileReader(file));
           
        } catch (IOException e) {                   
            e.printStackTrace();
        }     
        return br;
    }
    
    /*
     *  写到路径为filePath的文件，写的内容为o（需要重载toString函数），isAppend标记是否追加
     * */
    public static void writeToFile(String filePath, Object o, Boolean isAppend) {
    	
    	try {
            BufferedWriter bw = new BufferedWriter(new FileWriter(filePath, isAppend));

            bw.write(o.toString()+"\n");
            bw.close();
            
        } catch (IOException ex) {
            Logger.getLogger(o.getClass().getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    /*
     *   默认为追加写
     * */
    public static void writeToFile(String filePath, Object o) {
    	
    	writeToFile(filePath, o, true);
    }
    
    /*
     *   以segBegin和segEnd为段落起止的写文件
     * */
    public static void writeToFile(String filePath, Object o, Boolean isAppend, String segBegin, String segEnd) {
    	
    	try {
            BufferedWriter bw = new BufferedWriter(new FileWriter(filePath, isAppend));
            bw.write(segBegin);
            bw.write(o.toString());
            bw.write(segEnd);
            bw.close();
            
        } catch (IOException ex) {
            Logger.getLogger(o.getClass().getName()).log(Level.SEVERE, null, ex);
        }
    }
}
