package org.oscar.ai.lxc.data;

/**
 * @author LXC
 *
 */


/*
 *   һ������������ԭ���ʺʹ�������
 * */
public class AISubstitution {

	public String origWord;
	public String subsWord;
	
	public AISubstitution(String origWord, String subsWord){
		this.origWord = new String(origWord);
		this.subsWord = new String(subsWord);
	}
	
	@Override
	public String toString(){
		return subsWord + "/" + origWord;
	}
}
