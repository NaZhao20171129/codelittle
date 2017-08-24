package org.oscar.ai.lxc.main;

import java.io.BufferedReader;
import java.io.IOException;

import org.oscar.ai.lxc.data.AIClause;
import org.oscar.ai.lxc.data.AIClauseSet;
import org.oscar.ai.lxc.data.AIPrediFormula;
import org.oscar.ai.lxc.robinson.DeleteSetResolution;
import org.oscar.ai.lxc.robinson.Resolution;
import org.oscar.ai.lxc.unifier.AISimpleUnifier;
import org.oscar.xc.tools.OFileTool;

public class AllMain {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		
		// TODO Auto-generated method stub	
		
		BufferedReader br = OFileTool.generalReadFile("clauseSet.txt");	// ¶ÁÈ¡×Ó¾ä¼¯
		DeleteSetResolution dsr = new DeleteSetResolution();
		AIClauseSet aicSet = new AIClauseSet();
		String tmpStr = "";
		while (null != (tmpStr = br.readLine())){
			
			if (!tmpStr.equals("---")){
				aicSet.addClause(new AIClause(tmpStr));
			} else {
				
				dsr.ResolutionClauseSet(aicSet);
				System.out.println("\n---\n");
				OFileTool.writeToFile("result.txt", "\n---\n");
				aicSet = new AIClauseSet();
			}		
		}
	}

}
