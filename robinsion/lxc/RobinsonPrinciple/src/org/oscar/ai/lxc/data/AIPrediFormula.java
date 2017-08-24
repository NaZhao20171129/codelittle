package org.oscar.ai.lxc.data;

import java.util.ArrayList;
import java.util.List;

/**
 * @author LXC
 *
 */
/*
 *   谓词公式类，用于存储一个谓词公式，一个谓词公式有多个文字（Character）的（析取）集合组成
 *   加入了正负的概念
 * */
public class AIPrediFormula {

	private String prediName;	// 谓词名称，P(x, y, g(a))中的P
	private int sign;	// 用于标记正负，输入时用“-”号表示
	List<String> charList;	// 谓词内容，一些列文字的集合
	
	public AIPrediFormula(){
		this.prediName = "";
		sign = 1;
		this.charList = new ArrayList<String>();
	}
	
	public AIPrediFormula(String prediString){	// 利用字符串初始化谓词公式
		this.prediName = "";
		this.charList = new ArrayList<String>();
		stringToFormula(prediString);
	}
	
	private void stringToFormula(String prediString){	// 真正的字符串转为谓词公式函数，（这里不对字符串的合法性即顺序做检查）
		
		int begin = prediString.indexOf("(");	// 找到第一个左括号，为原子谓词的开始
		int end = prediString.lastIndexOf(")");	// 找到最后一个右括号，为原子谓词的结束
		
		prediName = prediString.substring(0, begin);	// 从0到第一个左括号为谓词公式的符号和名称
		if (prediName.startsWith("-")){	
			prediName = prediName.substring(1, prediName.length());
			sign = -1;
		} else {
			sign = 1;
		}
		
		String allChar = prediString.substring(begin+1, end);
		String[] allCharArray = allChar.split(",");	// 用","把原子谓词切开
		for (int i = 0; i < allCharArray.length; ++i){
			charList.add(new String(allCharArray[i].trim()));	// 把每个切开后的谓词放入charList中
		}
		
	}
	
	public List<String> getCharList(){
		return this.charList;
	}
	
	public AIPrediFormula(AIPrediFormula pf){
		this.prediName = pf.prediName;
		this.sign = pf.sign;
		this.charList = pf.charList;
	}
	
	public int getSize(){	// 获得谓词公式的谓词数量
		return this.charList.size();
	}
	
	public String getNthChar(Integer index){	//返回第i个谓词项
		return this.charList.get(index);
	}

	public String getPrediName(){
		return this.prediName;
	}
	
	/*
	 *   除了符号，完全一样
	 * */
	public boolean halfEqual(AIPrediFormula aipf){
		
		if (this.prediName.equals(aipf.prediName) && this.sign != aipf.sign){
			for (int i = 0; i < charList.size(); ++i){
				if (!charList.get(i).equals(aipf.charList.get(i)))
					return false;
			}
		} else {
			return false;
		}
		return true;
	}
	
	/*
	 *   符号相反，谓词名相同，内容可以不同
	 * */
	public boolean equalSignPrediName(AIPrediFormula aipf){
		if (this.prediName.equals(aipf.prediName) && this.sign != aipf.sign)
			return true;
		else
			return false;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		int charListValue = 0;
		if (charList != null){
			for (int i = 0; i < charList.size(); ++i){
				charListValue += charList.get(i).hashCode();
			}
		}
		result = prime * result
				+ charListValue;
		result = prime * result
				+ ((prediName == null) ? 0 : prediName.hashCode());
		result = prime * result + sign;
		
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		AIPrediFormula other = (AIPrediFormula) obj;
		if (charList == null) {
			if (other.charList != null)
				return false;
		} else {
			for (int i = 0; i < charList.size(); ++i){
				if (!charList.get(i).equals(other.charList.get(i)))
					return false;
			}
		}
		if (prediName == null) {
			if (other.prediName != null)
				return false;
		} else if (!prediName.equals(other.prediName))
			return false;
		
		if (sign != other.sign)
			return false;
		
		return true;
	}
	
	@Override
	public String toString(){	// 按照P(x,y,g(a))格式输出谓词公式
		String tmpStr = "";
		if (sign == 1)
			tmpStr += this.prediName + "(";
		else
			tmpStr += "-" + this.prediName + "(";
		
		for (int i = 0; i < charList.size(); ++i){
			tmpStr += charList.get(i) + ",";
		}
		tmpStr = tmpStr.substring(0, tmpStr.length() - 1);
		tmpStr += ")";
		return tmpStr;
	}
}
