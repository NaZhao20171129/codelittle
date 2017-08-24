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
	 *   ��һ���Ӿ伯���й�ᡣ���չ��ɹ��ǽ����˹�ᣬ��n���Ӿ伯��СΪ0��ʧ�ܣ��Ǳ���û���й�ᣬ��n���Ӿ��С��Ϊ0
	 * */
	public void ResolutionClauseSet(AIClauseSet clauseSet){
		
		AIClauseSet origClauseSet = new AIClauseSet();
		origClauseSet.addAll(clauseSet);	// Ϊ�˲��ı�ԭʼ���Ӿ伯����
		AIClauseSet newClauseSet = new AIClauseSet();
		AIClauseSet tmpClauseSet = new AIClauseSet();
		
		int resolutionCount = 0;
		boolean isResolution = true;
		
		System.out.println("ԭʼ�Ӿ伯Ϊ:");
		System.out.println(origClauseSet);
		
		newClauseSet = firstResolution(origClauseSet);	
		
		if (newClauseSet == null){
			System.out.println("���ʧ��");
			System.exit(0);
		} else if (newClauseSet.getSize() == 0){
			System.out.println("���ɹ�");
			System.exit(0);
		} else {
			++resolutionCount;
			System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
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
				System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				System.out.println(newClauseSet);
				System.out.println("���ܼ�����ᣬ���ʧ��");
				isResolution = false;
			} else if (newClauseSet.getSize() == 0){
				++resolutionCount;
				System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				System.out.println("����NIL�����ɹ�");
				isResolution = false;
			} else {
				++resolutionCount;
				System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				System.out.println(newClauseSet);
			}
			
		}
	}
	
	/*
	 *   ���е�һ�ι�ᣬ����һ��ʽ�ӡ���һ����ԭʼ�Ӿ伯���Խ�
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
		if (firstSuccess)	// ȷʵ�����˹�ᣬ�򷵻�һ���Ӿ伯
			return newClauseSet;
		else	// û�н��й�ᣬ���ؿգ�����ǿգ�����ʧ����
			return null;
	}
	
	/*
	 *   ���е�n�ι�ᣨn>1��������n+1��ʽ�ӡ���n�ι����n-1֮ǰ�������Ӿ伯���n���Ӿ伶���ӽ�
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
		
		if (firstSuccess)	// ȷʵ�����˹�ᣬ�򷵻�һ���Ӿ伯
			return newClauseSet;
		else	// û�н��й�ᣬ���ؿգ�����ǿգ�����ʧ����
			return null;
	}
	
	/*
	 *   �������Ӿ������һ���һ�����й�ᣬ���ع���Ľ��
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
				if (aipf1.halfEqual(aipf2)){	// ���ֱ�ӿ��Ժϲ�����ֱ��ɾ��
					isResolution = true;
					tmpClause.getPrediList().remove(j);
					tmpClause.getPrediList().remove(i);
					--i;
					break;
				} else if (aipf1.equals(aipf2)){	// ���������ȫ��ȣ������ֱ�Ӻϲ��ˣ�����û�н��к�һ����
					tmpClause.getPrediList().remove(j);
					--j;
				} else if (aipf1.getPrediName().equals(aipf2.getPrediName()) && aipf1.getSize() == aipf2.getSize()){
					isResolution = true;
					List<AISubstitution> aiSubList = aiSU.aiSimpleUnifier(aipf1, aipf2);
					if (aiSubList != null){
						for (int k = 0; k < aiSubList.size(); ++k){
							for (int l = 0; l < tmpClause.getSize(); ++l){
								aiSU.predSubstitute(tmpClause.getNthFormula(l), aiSubList.get(k));	// �����еĶ������滻��Ȼ��ɾ��
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
