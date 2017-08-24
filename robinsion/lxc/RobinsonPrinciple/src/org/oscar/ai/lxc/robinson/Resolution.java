package org.oscar.ai.lxc.robinson;

/**
 * @author LXC
 *
 */
import java.util.List;

import org.oscar.ai.lxc.data.AIClause;
import org.oscar.ai.lxc.data.AIClauseSet;
import org.oscar.ai.lxc.data.AIPrediFormula;
import org.oscar.ai.lxc.data.AISubstitution;
import org.oscar.ai.lxc.unifier.AISimpleUnifier;

public class Resolution {

	/*
	 *   对一个子句集进行归结。最终归结成功是进行了归结，且n级子句集大小为0；失败，是本次没进行归结，且n级子句大小不为0
	 * */
	public void ResolutionClauseSet(AIClauseSet clauseSet){
		
		AIClauseSet origClauseSet = new AIClauseSet();
		origClauseSet.addAll(clauseSet);	// 为了不改变原始的子句集内容
		AIClauseSet newClauseSet = new AIClauseSet();
		AIClauseSet tmpClauseSet = new AIClauseSet();
		
		int resolutionCount = 0;
		boolean isResolution = true;
		
		System.out.println("原始子句集为:");
		System.out.println(origClauseSet);
		
		newClauseSet = firstResolution(origClauseSet);	
		
		if (newClauseSet == null){
			System.out.println("归结失败");
			System.exit(0);
		} else if (newClauseSet.getSize() == 0){
			System.out.println("归结成功");
			System.exit(0);
		} else {
			++resolutionCount;
			System.out.println(resolutionCount + "级归结式中子句为:");
			System.out.println(newClauseSet);
		}
		
		while(isResolution){
			
			origClauseSet.addAll(tmpClauseSet);
			tmpClauseSet = newClauseSet;
			newClauseSet = new AIClauseSet();
			
			newClauseSet = nResolution(origClauseSet, tmpClauseSet);
			newClauseSet.rmSameClause();

			if (newClauseSet == null){
				++resolutionCount;
				System.out.println(resolutionCount + "级归结式中子句为:");
				System.out.println(newClauseSet);
				System.out.println("不能继续归结，归结失败");
				isResolution = false;
			} else if (newClauseSet.getSize() == 0){
				++resolutionCount;
				System.out.println(resolutionCount + "级归结式中子句为:");
				System.out.println("出现NIL，归结成功");
				isResolution = false;
			} else {
				++resolutionCount;
				System.out.println(resolutionCount + "级归结式中子句为:");
				System.out.println(newClauseSet);
			}
			
		}
	}
	
	/*
	 *   进行第一次归结，生成一级式子。第一次是原始子句集的自交
	 * */
	private AIClauseSet firstResolution(AIClauseSet clauseSet){
		
		AIClauseSet newClauseSet = new AIClauseSet();
		boolean firstSuccess = false;
		
		for (int i = 0; i < clauseSet.getSize(); ++i){
			for (int j = i+1; j < clauseSet.getSize(); ++j){
				
				AIClause tmpClause = new AIClause();
				boolean isTwoSuccess = ResolutionTwoClause(clauseSet.getNthClause(i), clauseSet.getNthClause(j), tmpClause);
				if (isTwoSuccess){
					firstSuccess = true;
					if (tmpClause.getSize() != 0){
						newClauseSet.addClause(tmpClause);
					}
				}
					
			}
		}
		if (firstSuccess)	// 确实进行了归结，则返回一级子句集
			return newClauseSet;
		else	// 没有进行归结，返回空，如果是空，则归结失败了
			return null;
	}
	
	/*
	 *   进行第n次归结（n>1），生成n+1级式子。第n次归结是n-1之前的所有子句集与第n级子句级的杂交
	 * */
	private AIClauseSet nResolution(AIClauseSet n_1clauseSet, AIClauseSet nclauseSet){
		
		AIClauseSet newClauseSet = new AIClauseSet();
		boolean firstSuccess = false;
		
		for (int i = 0; i < n_1clauseSet.getSize(); ++i){
			for (int j = 0; j < nclauseSet.getSize(); ++j){
				
				AIClause tmpClause = new AIClause();
				boolean isTwoSuccess = ResolutionTwoClause(n_1clauseSet.getNthClause(i), nclauseSet.getNthClause(j), tmpClause);
				if (isTwoSuccess){
					firstSuccess = true;
					if (tmpClause.getSize() != 0){
						newClauseSet.addClause(tmpClause);
					}
				}	
			}
		}
		
		if (firstSuccess)	// 确实进行了归结，则返回一级子句集
			return newClauseSet;
		else	// 没有进行归结，返回空，如果是空，则归结失败了
			return null;
	}
	
	/*
	 *   对两个子句调用最一般合一，进行归结，返回归结后的结果
	 * */
	private boolean ResolutionTwoClause(AIClause aic1, AIClause aic2, AIClause tmpClause){
		
		AISimpleUnifier aiSU = new AISimpleUnifier();
		boolean isResolution = false;
		tmpClause.addAll(aic1);
		tmpClause.addAll(aic2);
		
		for (int i = 0; i < tmpClause.getSize(); ++i){
			
			AIPrediFormula aipf1 = tmpClause.getNthFormula(i);
			for (int j = i+1; j < tmpClause.getSize(); ++j){
				
				AIPrediFormula aipf2 = tmpClause.getNthFormula(j);
				if (aipf1.halfEqual(aipf2)){	// 如果直接可以合并，则直接删除
					isResolution = true;
					tmpClause.getPrediList().remove(j);
					tmpClause.getPrediList().remove(i);
					--i;
					break;
				} else if (aipf1.equals(aipf2)){	// 如果两个完全相等，则可以直接合并了，但是没有进行合一操作
					tmpClause.getPrediList().remove(j);
					--j;
				} else if (aipf1.getPrediName().equals(aipf2.getPrediName()) && aipf1.getSize() == aipf2.getSize()){
					isResolution = true;
					List<AISubstitution> aiSubList = aiSU.aiSimpleUnifier(aipf1, aipf2);
					if (aiSubList != null){
						for (int k = 0; k < aiSubList.size(); ++k){
							for (int l = 0; l < tmpClause.getSize(); ++l){
								aiSU.predSubstitute(tmpClause.getNthFormula(l), aiSubList.get(k));	// 把所有的都进行替换，然后删除
							}
						}
					}
					tmpClause.getPrediList().remove(j);
					tmpClause.getPrediList().remove(i);
					--i;
					break;
				}
			}
		}
		
		return isResolution;
	}

}
