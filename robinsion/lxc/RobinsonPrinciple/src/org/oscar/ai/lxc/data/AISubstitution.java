package org.oscar.ai.lxc.data;

/**
 * @author LXC
 *
 */


/*
 *   一个代换，包括原单词和代换单词
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
