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
import org.oscar.xc.tools.OFileTool;

public class DeleteSetResolution {

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
		OFileTool.writeToFile("result.txt", "ԭʼ�Ӿ伯Ϊ:");
		OFileTool.writeToFile("result.txt", origClauseSet);
		
		doDelete(origClauseSet);	// �ж�ɾ����
		
		newClauseSet = firstResolution(origClauseSet);	
		
		if (newClauseSet == null){
			System.out.println("���ʧ��");
			OFileTool.writeToFile("result.txt", "���ʧ��");
			System.exit(0);
		} else if (newClauseSet.getSize() == 0){
			System.out.println("���ɹ�");
			OFileTool.writeToFile("result.txt", "���ɹ�");
			System.exit(0);
		} else {
			++resolutionCount;
			System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
			System.out.println(newClauseSet);
			OFileTool.writeToFile("result.txt", resolutionCount + "�����ʽ���Ӿ�Ϊ:");
			OFileTool.writeToFile("result.txt", newClauseSet);
		}
		
		while(isResolution){
			
			origClauseSet.addAll(tmpClauseSet);
			doDelete(origClauseSet);	// �ж�ɾ����
			tmpClauseSet = newClauseSet;
			newClauseSet = new AIClauseSet();
			
			newClauseSet = nResolution(origClauseSet, tmpClauseSet);
			newClauseSet.rmSameClause();

			if (newClauseSet == null){
				++resolutionCount;
				System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				System.out.println(newClauseSet);
				System.out.println("���ܼ�����ᣬ���ʧ��");
				
				OFileTool.writeToFile("result.txt", resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				OFileTool.writeToFile("result.txt", newClauseSet);
				OFileTool.writeToFile("result.txt", "���ܼ�����ᣬ���ʧ��");
				
				isResolution = false;
			} else if (newClauseSet.getSize() == 0){
				++resolutionCount;
				System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				System.out.println("����NIL�����ɹ�");
				
				OFileTool.writeToFile("result.txt", resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				OFileTool.writeToFile("result.txt", "����NIL�����ɹ�");
				
				isResolution = false;
			} else {
				++resolutionCount;
				System.out.println(resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				System.out.println(newClauseSet);
				
				OFileTool.writeToFile("result.txt", resolutionCount + "�����ʽ���Ӿ�Ϊ:");
				OFileTool.writeToFile("result.txt", newClauseSet);
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
	
	/*
	 *   �ж�һ���Ӿ��Ƿ��ڵ�ǰ���Ӿ伯�д��ڴ�����
	 * */
	public boolean isComplementary(AIClause aic1, AIClauseSet curClauseSet){
		
		boolean breakTwo = false;
		int complementCount = 0;
		
		for (int i = 0; i < aic1.getSize(); ++i){
			AIPrediFormula aipf1 = aic1.getNthFormula(i);
			breakTwo = false;
			
			for (int j = 0; j < curClauseSet.getSize(); ++j){
				AIClause aic2 = curClauseSet.getNthClause(j);
				for (int k = 0; k < aic2.getSize(); ++k){
					AIPrediFormula aipf2 = aic2.getNthFormula(k);
					if (aipf1.equalSignPrediName(aipf2)){
						++complementCount;
						breakTwo = true;
						break;
					}
				}
				if (breakTwo)
					break;
			}
		}
		
		if (complementCount < aic1.getSize())
			return false;
		else
			return true;
	}
	
	/*
	 *   ɾ��������
	 * */
	private void deletePureWord(AIClauseSet curClauseSet){
		
		for (int i = 0; i < curClauseSet.getSize(); ++i){
			AIClause aic = curClauseSet.getNthClause(i);
			boolean isComplement = this.isComplementary(aic, curClauseSet);
			if (isComplement == false){
				System.out.println("ɾ���Ӿ�" + aic + "���ڴ�����");
				OFileTool.writeToFile("result.txt", "ɾ���Ӿ�" + aic + "���ڴ�����");
				
				curClauseSet.getClauseList().remove(i);
				--i;
			}
		}
	}
	
	/*
	 *   ɾ������ʽ
	 * */
	private void deleteTautology(AIClauseSet curClauseSet){
		
		boolean breakTwo = false;
		boolean isTautology = false;
		
		for (int i = 0; i < curClauseSet.getSize(); ++i){
			AIClause aic = curClauseSet.getNthClause(i);
			for (int j = 0; j < aic.getSize(); ++j){
				for (int k = j+1; k < aic.getSize(); ++k){
					if (aic.getNthFormula(j).halfEqual(aic.getNthFormula(k))){
						breakTwo = true;
						isTautology = true;
						break;
					}
				}
				if (breakTwo)
					break;
			}
			if (isTautology){
				System.out.println("ɾ���Ӿ�" + aic + "��������ʽ");
				OFileTool.writeToFile("result.txt", "ɾ���Ӿ�" + aic + "��������ʽ");
				
				curClauseSet.getClauseList().remove(i);
				--i;
			}
			breakTwo = false;
			isTautology = false;
		}
	}
	
	private void doDelete(AIClauseSet curClauseSet){
		deletePureWord(curClauseSet);
		deleteTautology(curClauseSet);
	}
}
