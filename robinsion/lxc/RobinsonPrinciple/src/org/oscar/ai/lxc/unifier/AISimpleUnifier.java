package org.oscar.ai.lxc.unifier;

import java.util.ArrayList;
import java.util.List;

import org.oscar.ai.lxc.data.AIPrediFormula;
import org.oscar.ai.lxc.data.AISubstitution;

public class AISimpleUnifier {

	/*
	 *   做替换，每次看一下括号里的是否能替换，但没实现对L(x,y,g(x,y))中的x替换
	 * */
	private Boolean doSubstitute(List<String> aiCharList, AISubstitution aiSub){
		
		Boolean isSubSuccess = false;
		
		for (int i = 0; i < aiCharList.size(); ++i){
			
			String aiChar = aiCharList.get(i);	// 获得第i个文字
			
			if (aiChar.equals(aiSub.origWord)){	// 如果文字和替换的正好相同，则直接替换
				
				isSubSuccess = true;
				aiCharList.set(i, aiSub.subsWord);
				
			} else {	// 否则需要看文字内部是否有满足的替换，比如替换g(f(x))中的x
				
				String aiStr = aiChar;
				String aiSubOrig = aiSub.origWord;

				int firstBracket = aiStr.indexOf("(");	
				int lastBracket = aiStr.lastIndexOf(")");
				
				int finalSubBegin = firstBracket;
				int finalSubEnd = lastBracket;
				
				while (lastBracket != firstBracket){

					aiStr = aiStr.substring(firstBracket+1, lastBracket);	// 每次拆掉一组括号
					if (aiStr.equals(aiSubOrig)) {
						String newChar = aiChar.substring(0, finalSubBegin+1) + aiSub.subsWord 
						+ aiChar.substring(finalSubEnd, aiChar.length());	//实际上就是把finalSubBegin到finalSubEnd的用aiSub.subsWord替换
						aiCharList.set(i, new String(newChar));
						isSubSuccess = true;
						break;
					} else {
						firstBracket = aiStr.indexOf("(");	
						lastBracket = aiStr.lastIndexOf(")");

						finalSubEnd = finalSubBegin + lastBracket + 1;	// 计算实际的在原始文字中的替换位置。
						finalSubBegin += firstBracket + 1;
					}	
				}
			}
		}
		
		return isSubSuccess;
	}
	
	/*
	 *   对一个谓词公式，利用某个代换进行代换
	 * */
	public Boolean predSubstitute(AIPrediFormula aiFormula, AISubstitution aiSub){
		
		Boolean isSubSuccess = false;
		
		List<String> aiCharList = aiFormula.getCharList();
		isSubSuccess = doSubstitute(aiCharList, aiSub);
		
		return isSubSuccess;
	}
	
	/*
	 *   寻找第一个差异集，作为替换目标
	 * */
	public AISubstitution findFirstDiffSet(AIPrediFormula aiF1, AIPrediFormula aiF2){
		
		if (aiF1.getSize() != aiF2.getSize()){	// 如果大小不同，则无法求解，返回空
			return null;
		} else {
			for (int i = 0; i < aiF1.getSize(); ++i){
				if(!aiF1.getNthChar(i).equals(aiF2.getNthChar(i))){
					return getRealSub(aiF1.getNthChar(i), aiF2.getNthChar(i));
				}
			}
		}
		return null;	// 如果全都相同，返回空
	}
	
	/*
	 *   在AIChar中寻找真正的替换的位置，并生成替换集
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
	 *   找到不同的一个串，比如g(f(x))，begin是位置2的话，则返回位置6，就是说从2到6就是是真正需要的
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
		
		end += bracketCount;	// 加上n个左括号的位置，就是结束了，还得让出一位
		return end;	
	}
	
	/*
	 *   判断变量和常量
	 * */
	private int varConst(String aiStr)
	{
		if(aiStr.length()==0) return 0; //空
		if(aiStr.length()==1&&aiStr.charAt(0)>='a'&&aiStr.charAt(0)<='g')  return 1; //常量
		if(aiStr.length()>1)
		{
			int i=0;
			while(i<aiStr.length()&&aiStr.charAt(i)!='(')
				i++;
			String aiSS=aiStr.substring(i+1,aiStr.length()-1);	//抽取s中的匹配的()中的子串
			return varConst(aiSS);
		}
		else return 2;
	}

	/*
	 *   最一般合一接口，输入两个谓词，返回差异集
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
