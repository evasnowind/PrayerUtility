package algorithm.tree;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Stack;

public class TraverseAlgorithm {

	public void visitNode(TreeNode node){
		System.out.println(node.val);
	}
	
	public void preOrderTraverseByRecursive(TreeNode root){
		if(null != root){
			visitNode(root);
			preOrderTraverseByRecursive(root.left);
			preOrderTraverseByRecursive(root.right);
		}
	}
	
	//TODO BE TEST
	public void preOrderTraverse(TreeNode root){
		Stack<TreeNode> stack = new Stack<TreeNode>();
		TreeNode node = root;
		while(null != node || !stack.isEmpty()){
			if(null != node){
				stack.push(node);
				visitNode(node);
				node = node.left;
			} else {
				node = stack.pop();
				node = node.right;
			}
		}
	}
	
	public void inOrderTraverseByRecursive(TreeNode root){
		if(null != root){
			inOrderTraverseByRecursive(root.left);
			visitNode(root);
			inOrderTraverseByRecursive(root.right);
		}
	}
	
	/*
	 * 与先序遍历非递归实现基本一致，只是visitNode调用时机不同而已
	 */
	public void inOrderTraverse(TreeNode root){
		Stack<TreeNode> stack = new Stack<TreeNode>();
		TreeNode node = root;
		while(null != node || !stack.isEmpty()){
			if(null != node){
				stack.push(node);
				node = node.left;
			} else {
				node = stack.pop();
				visitNode(node);
				node = node.right;
			}
		}
	}
	
	public void postOrderTraverseByRecursive(TreeNode root){
		if(null != root){
			postOrderTraverseByRecursive(root.left);
			postOrderTraverseByRecursive(root.right);
			visitNode(root);
		}
	}
	
	public void postOrderTraverse(TreeNode root){
		if(null == root){
			return;
		}
		
		TreeNode curNode = root;
		TreeNode prePopNode = null;
		Stack<TreeNode> stack = new Stack<TreeNode>();
		while(null != curNode || !stack.isEmpty()){
			while(null != curNode){
				stack.push(curNode);
				curNode = curNode.left;
			}
			curNode = stack.peek();
			if(null != curNode.right && curNode.right != prePopNode){
				curNode = curNode.right;
			} else {
				visitNode(curNode);
				prePopNode = stack.pop();
				curNode = null;
			}
		}
	}
	
	//level traverse
	public List<List<Integer>> levelOrder(TreeNode root) {
        Queue<TreeNode> queue = new LinkedList<TreeNode>();
        List<List<Integer>> wrapList = new LinkedList<List<Integer>>();
        
        if(root == null) return wrapList;
        
        queue.offer(root);
        while(!queue.isEmpty()){
            int levelNum = queue.size();
            List<Integer> subList = new LinkedList<Integer>();
            for(int i=0; i<levelNum; i++) {
                if(queue.peek().left != null) queue.offer(queue.peek().left);
                if(queue.peek().right != null) queue.offer(queue.peek().right);
                subList.add(queue.poll().val);
            }
            wrapList.add(subList);
        }
        return wrapList;
    }
	
	public List<List<Integer>> levelOrder2(TreeNode root) {
        List<List<Integer>> result = new ArrayList<List<Integer>>();
		if(null == root) return result;
		Queue<TreeNode> queue = new LinkedList<TreeNode>();
		
		queue.add(root);
		
		while(!queue.isEmpty()){
			queue.add(null);
			TreeNode tn = queue.remove();
			ArrayList<Integer> levelList = new ArrayList<Integer>();
			while(null != tn){
				levelList.add(tn.val);
				if(null != tn.left) queue.add(tn.left);
				if(null != tn.right) queue.add(tn.right);
				tn = queue.remove();
			}
			result.add(levelList);
		}
        return result;
    }
}
