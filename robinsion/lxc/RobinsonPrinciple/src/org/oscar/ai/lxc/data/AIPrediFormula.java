package org.oscar.ai.lxc.data;

import java.util.ArrayList;
import java.util.List;

/**
 * @author LXC
 *
 */
/*
 *   ν�ʹ�ʽ�࣬���ڴ洢һ��ν�ʹ�ʽ��һ��ν�ʹ�ʽ�ж�����֣�Character���ģ���ȡ���������
 *   �����������ĸ���
 * */
public class AIPrediFormula {

	private String prediName;	// ν�����ƣ�P(x, y, g(a))�е�P
	private int sign;	// ���ڱ������������ʱ�á�-���ű�ʾ
	List<String> charList;	// ν�����ݣ�һЩ�����ֵļ���
	
	public AIPrediFormula(){
		this.prediName = "";
		sign = 1;
		this.charList = new ArrayList<String>();
	}
	
	public AIPrediFormula(String prediString){	// �����ַ�����ʼ��ν�ʹ�ʽ
		this.prediName = "";
		this.charList = new ArrayList<String>();
		stringToFormula(prediString);
	}
	
	private void stringToFormula(String prediString){	// �������ַ���תΪν�ʹ�ʽ�����������ﲻ���ַ����ĺϷ��Լ�˳������飩
		
		int begin = prediString.indexOf("(");	// �ҵ���һ�������ţ�Ϊԭ��ν�ʵĿ�ʼ
		int end = prediString.lastIndexOf(")");	// �ҵ����һ�������ţ�Ϊԭ��ν�ʵĽ���
		
		prediName = prediString.substring(0, begin);	// ��0����һ��������Ϊν�ʹ�ʽ�ķ��ź�����
		if (prediName.startsWith("-")){	
			prediName = prediName.substring(1, prediName.length());
			sign = -1;
		} else {
			sign = 1;
		}
		
		String allChar = prediString.substring(begin+1, end);
		String[] allCharArray = allChar.split(",");	// ��","��ԭ��ν���п�
		for (int i = 0; i < allCharArray.length; ++i){
			charList.add(new String(allCharArray[i].trim()));	// ��ÿ���п����ν�ʷ���charList��
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
	
	public int getSize(){	// ���ν�ʹ�ʽ��ν������
		return this.charList.size();
	}
	
	public String getNthChar(Integer index){	//���ص�i��ν����
		return this.charList.get(index);
	}

	public String getPrediName(){
		return this.prediName;
	}
	
	/*
	 *   ���˷��ţ���ȫһ��
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
	 *   �����෴��ν������ͬ�����ݿ��Բ�ͬ
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
	public String toString(){	// ����P(x,y,g(a))��ʽ���ν�ʹ�ʽ
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
