package org.oscar.ai.lxc.data;

import java.util.ArrayList;
import java.util.List;

/**
 * @author LXC
 *
 */
/*
 *   �Ӿ伯�࣬����һЩ�е��Ӿ䣬���ڹ��Ȳ���
 * */

public class AIClauseSet {

	List<AIClause> clauseList;
	
	public AIClauseSet(){
		this.clauseList = new ArrayList<AIClause>();
	}
	
	public AIClauseSet(AIClauseSet aics){
		clauseList = aics.clauseList;
	}

	public List<AIClause> getClauseList() {
		return clauseList;
	}

	public void setClauseList(List<AIClause> clauseList) {
		this.clauseList = clauseList;
	}
	
	public int getSize(){
		return this.clauseList.size();
	}
	
	/*
	 *   ���Ӿ伯�������Ӿ�
	 * */
	public void addClause(AIClause aic){
		this.clauseList.add(aic);
	}
	
	public AIClause getNthClause(int i){
		if (i < clauseList.size())
			return clauseList.get(i);
		else
			return null;
	}
	
	/*
	 *   ��һ���Ӿ伯���Ӿ䶼���뵽��һ���Ӿ伯��
	 * */
	public void addAll(AIClauseSet aicSet){
		this.clauseList.addAll(aicSet.clauseList);
	}
	
	@Override
	public String toString(){
		String tmpStr = "";
		for (int i = 0; i < getSize(); ++i)
			tmpStr += getNthClause(i) + "\n";
		return tmpStr;
	}
	
	/*
	 *   �Ƴ���ͬ��
	 * */
	public void rmSameClause(){
		for (int i = 0; i < this.getSize(); ++i){
			for (int j = i+1; j < this.getSize(); ++j){
				if (this.clauseList.get(i).equals(clauseList.get(j))){
					clauseList.remove(j);
					--j;
				}
			}
		}
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result
				+ ((clauseList == null) ? 0 : clauseList.hashCode());
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
		AIClauseSet other = (AIClauseSet) obj;
		if (clauseList == null) {
			if (other.clauseList != null)
				return false;
		} else {
			if (this.getSize() != other.getSize())
				return false;
			for (int i = 0; i < this.getSize(); ++i){
				if (!this.clauseList.get(i).equals(other.clauseList.get(i)))
					return false;
			}
		}
		return true;
	}
	
	
}
