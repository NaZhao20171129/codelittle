package org.oscar.ai.lxc.unifier;

import java.util.ArrayList;
import java.util.List;

import org.oscar.ai.lxc.data.AIPrediFormula;
import org.oscar.ai.lxc.data.AISubstitution;

public class AISimpleUnifier {

	/*
	 *   ���滻��ÿ�ο�һ����������Ƿ����滻����ûʵ�ֶ�L(x,y,g(x,y))�е�x�滻
	 * */
	private Boolean doSubstitute(List<String> aiCharList, AISubstitution aiSub){
		
		Boolean isSubSuccess = false;
		
		for (int i = 0; i < aiCharList.size(); ++i){
			
			String aiChar = aiCharList.get(i);	// ��õ�i������
			
			if (aiChar.equals(aiSub.origWord)){	// ������ֺ��滻��������ͬ����ֱ���滻
				
				isSubSuccess = true;
				aiCharList.set(i, aiSub.subsWord);
				
			} else {	// ������Ҫ�������ڲ��Ƿ���������滻�������滻g(f(x))�е�x
				
				String aiStr = aiChar;
				String aiSubOrig = aiSub.origWord;

				int firstBracket = aiStr.indexOf("(");	
				int lastBracket = aiStr.lastIndexOf(")");
				
				int finalSubBegin = firstBracket;
				int finalSubEnd = lastBracket;
				
				while (lastBracket != firstBracket){

					aiStr = aiStr.substring(firstBracket+1, lastBracket);	// ÿ�β��һ������
					if (aiStr.equals(aiSubOrig)) {
						String newChar = aiChar.substring(0, finalSubBegin+1) + aiSub.subsWord 
						+ aiChar.substring(finalSubEnd, aiChar.length());	//ʵ���Ͼ��ǰ�finalSubBegin��finalSubEnd����aiSub.subsWord�滻
						aiCharList.set(i, new String(newChar));
						isSubSuccess = true;
						break;
					} else {
						firstBracket = aiStr.indexOf("(");	
						lastBracket = aiStr.lastIndexOf(")");

						finalSubEnd = finalSubBegin + lastBracket + 1;	// ����ʵ�ʵ���ԭʼ�����е��滻λ�á�
						finalSubBegin += firstBracket + 1;
					}	
				}
			}
		}
		
		return isSubSuccess;
	}
	
	/*
	 *   ��һ��ν�ʹ�ʽ������ĳ���������д���
	 * */
	public Boolean predSubstitute(AIPrediFormula aiFormula, AISubstitution aiSub){
		
		Boolean isSubSuccess = false;
		
		List<String> aiCharList = aiFormula.getCharList();
		isSubSuccess = doSubstitute(aiCharList, aiSub);
		
		return isSubSuccess;
	}
	
	/*
	 *   Ѱ�ҵ�һ�����켯����Ϊ�滻Ŀ��
	 * */
	public AISubstitution findFirstDiffSet(AIPrediFormula aiF1, AIPrediFormula aiF2){
		
		if (aiF1.getSize() != aiF2.getSize()){	// �����С��ͬ�����޷���⣬���ؿ�
			return null;
		} else {
			for (int i = 0; i < aiF1.getSize(); ++i){
				if(!aiF1.getNthChar(i).equals(aiF2.getNthChar(i))){
					return getRealSub(aiF1.getNthChar(i), aiF2.getNthChar(i));
				}
			}
		}
		return null;	// ���ȫ����ͬ�����ؿ�
	}
	
	/*
	 *   ��AIChar��Ѱ���������滻��λ�ã��������滻��
	 * */
	private AISubstitution getRealSub(String aiC1, String aiC2){
		
		String aiC1Str = aiC1;
		String aiC2Str = aiC2;

		int length = aiC1Str.length() < aiC2Str.length() ? aiC1Str.length() : aiC2Str.length();
		int i = 0;
		while (i < length){
			if (aiC1Str.charAt(i) != aiC2Str.charAt(i)){
				break;
			}
			++i;
		}
		 aiC1Str.substring(0,1);
		aiC1Str = aiC1Str.substring(i, subEndIndex(aiC1Str, i));
		aiC2Str = aiC2Str.substring(i, subEndIndex(aiC2Str, i));
		int aiC1Var = varConst(aiC1Str);
		int aiC2Var = varConst(aiC2Str);
		
		if (aiC1Var == 0 && aiC2Var == 0)
			return null;
		else if (aiC1Var == 1 && aiC2Var == 1)
			return null;
		else if (aiC1Var == 2){
			if (aiC2Var == 1)
				return new AISubstitution(aiC1Str, aiC2Str);
			else if (aiC2Str.length() >= aiC1Str.length())
				return new AISubstitution(aiC1Str, aiC2Str);
		} else if (aiC2Var == 2){
			if (aiC1Var == 1)
				return new AISubstitution(aiC2Str, aiC1Str);
			else if (aiC1Str.length() >= aiC2Str.length())
				return new AISubstitution(aiC2Str, aiC1Str);
		}
		return null;
	}
	
	/*
	 *   �ҵ���ͬ��һ����������g(f(x))��begin��λ��2�Ļ����򷵻�λ��6������˵��2��6������������Ҫ��
	 * */
	private int subEndIndex(String aiStr, int begin){
		
		int end = begin;
		int bracketCount = 0;
		
		for(int i = begin; i < aiStr.length(); ++i){
			
			if (aiStr.charAt(i) == '('){
				++bracketCount;
			} else if (aiStr.charAt(i) == ')'){
				break;
			}
			++end;
		}
		
		end += bracketCount;	// ����n�������ŵ�λ�ã����ǽ����ˣ������ó�һλ
		return end;	
	}
	
	/*
	 *   �жϱ����ͳ���
	 * */
	private int varConst(String aiStr)
	{
		if(aiStr.length()==0) return 0; //��
		if(aiStr.length()==1&&aiStr.charAt(0)>='a'&&aiStr.charAt(0)<='g')  return 1; //����
		if(aiStr.length()>1)
		{
			int i=0;
			while(i<aiStr.length()&&aiStr.charAt(i)!='(')
				i++;
			String aiSS=aiStr.substring(i+1,aiStr.length()-1);	//��ȡs�е�ƥ���()�е��Ӵ�
			return varConst(aiSS);
		}
		else return 2;
	}

	/*
	 *   ��һ���һ�ӿڣ���������ν�ʣ����ز��켯
	 * */
	public List<AISubstitution> aiSimpleUnifier(AIPrediFormula aiP1, AIPrediFormula aiP2){

		List<AISubstitution> aiSubList = new ArrayList<AISubstitution>();
		AISimpleUnifier aiSU = new AISimpleUnifier();
		AISubstitution tmpAISub = aiSU.findFirstDiffSet(aiP1, aiP2);

		if (tmpAISub == null){
			System.out.println("cannot do simple unifier");
			return null;
		}
		int subCount = 0;
		while (tmpAISub != null){
			aiSubList.add(tmpAISub);
			aiSU.predSubstitute(aiP1, tmpAISub);
			aiSU.predSubstitute(aiP2, tmpAISub);
			
			++subCount;
			String tmpStr = "";
			for (int i = 0; i <aiSubList.size(); ++i){
				tmpStr += aiSubList.get(i) + " ";
			}
	//		System.out.println("now different sets are : {" + tmpStr + "}" );
	//		System.out.println("substitute " + subCount + " : " + aiP1);
	//		System.out.println("               " + aiP2);
			
			tmpAISub = aiSU.findFirstDiffSet(aiP1, aiP2);	
		}
		
		return aiSubList;
	}
}
