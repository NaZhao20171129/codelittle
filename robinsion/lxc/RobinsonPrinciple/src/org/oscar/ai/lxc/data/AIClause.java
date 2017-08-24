package org.oscar.ai.lxc.data;

import java.util.ArrayList;
import java.util.List;

/**
 * @author LXC
 *
 */
/*
 *   子句类，一个子句是谓词公式的析取
 * */

public class AIClause {

	List<AIPrediFormula> prediList;
	
	public AIClause(){
		this.prediList = new ArrayList<AIPrediFormula>();
	}
	
	public AIClause(AIClause aic){
		prediList = aic.prediList;
	}
	
	public AIClause(String strClause){
		this.prediList = new ArrayList<AIPrediFormula>();
		String[] strArray = strClause.split("#");
		for (int i = 0; i < strArray.length; ++i){
			prediList.add(new AIPrediFormula(strArray[i]));
		}
	}
	
	public AIClause(AIPrediFormula...predicates){	// 传入n个谓词公式，形成子句
		this.prediList = new ArrayList<AIPrediFormula>();
		for (int i = 0; i < predicates.length; ++i){
			this.prediList.add(predicates[i]);
		}
	}

	public List<AIPrediFormula> getPrediList() {
		return prediList;
	}

	public void setPrediList(List<AIPrediFormula> prediList) {
		this.prediList = prediList;
	}
	
	public int getSize(){
		return this.prediList.size();
	}
	
	/*
	 *   在子句中增加析取的谓词公式
	 * */
	public void addPrediFormula(AIPrediFormula aipf){
		this.prediList.add(aipf);
	}
	
	/*
	 *   把一个子句都加入到另一个子句中
	 * */
	public void addAll(AIClause aic){
			this.prediList.addAll(aic.getPrediList());
	}
	
	public AIPrediFormula getNthFormula(int i){
		if (i < prediList.size())
			return prediList.get(i);
		else
			return null;
	}
	
	@Override
	public String toString(){
		String tmpStr = "";
		for (int i = 0; i < this.getSize(); ++i){
			if (i != this.getSize() - 1)
				tmpStr += getNthFormula(i) + "#";
			else 
				tmpStr += getNthFormula(i);
		}
		return tmpStr;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result
				+ ((prediList == null) ? 0 : prediList.hashCode());
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
		AIClause other = (AIClause) obj;
		if (prediList == null) {
			if (other.prediList != null)
				return false;
		} else {
			if (this.getSize() != other.getSize())
				return false;
			for (int i = 0; i < this.getSize(); ++i){
				if (!this.prediList.get(i).equals(other.prediList.get(i)))
					return false;
			}
		}
		return true;
	}

}
