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
     * ����ֹ��Ƕ�ȡ�ļ������ļ����ݴ洢��List�С�
     * filePathΪ�ļ�·����segBeginΪ����ʼ��ǣ�segEndΪ�ν�����ǣ�segSepTag�����趨���ֶ���ÿ�еı��
     */
    public static List<String> readFileSegments(String filePath, String segBegin, String segEnd, String segSepTag) {
        
        List<String> segList = new ArrayList<String>();
        
        File file = new File(filePath); // ��ȡ�ļ�      
        
        if(!file.exists() || !file.canRead()){	// �ж��ļ��Ƿ�ɶ�������
        	System.out.println("�ļ������ڻ򲻿ɶ�");
        	return segList;
        }
        
        BufferedReader br = null;      
        try{
            br = new BufferedReader(new FileReader(file));
            
            String tmpStr = ""; // ��ʱ�洢�ļ���һ��            
            String segStr = ""; // �洢�ļ���һ��            
            Boolean addStrFlag = false; // ��ȡ������
            
            while((tmpStr=br.readLine()) != null)
            {
                if (tmpStr.startsWith(segBegin)) {  // ���к�����ʼ����ǿ�ʼ�洢�ļ�����                  
                	addStrFlag = true;
                }
                
                if (addStrFlag){    // �ļ�������������segStr�У���segSepTag��Ƿָÿһ�С�
                    segStr = segStr + tmpStr + segSepTag;
                }
                
                if (tmpStr.startsWith(segEnd)) {    // ������������β��ǣ���segStr����List�У���ʼ�����                   segList.add(segStr);
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
     *   �������ļ�������buffer
     * */
    public static BufferedReader generalReadFile(String filePath) {
    	
    	File file = new File(filePath);
    	BufferedReader br = null;  
    	
    	if(!file.exists() || !file.canRead()){	// �ж��ļ��Ƿ�ɶ�������
        	System.out.println("�ļ������ڻ򲻿ɶ�");
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
     *  д��·��ΪfilePath���ļ���д������Ϊo����Ҫ����toString��������isAppend����Ƿ�׷��
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
     *   Ĭ��Ϊ׷��д
     * */
    public static void writeToFile(String filePath, Object o) {
    	
    	writeToFile(filePath, o, true);
    }
    
    /*
     *   ��segBegin��segEndΪ������ֹ��д�ļ�
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
