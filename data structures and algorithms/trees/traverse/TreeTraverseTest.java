package algorithm.tree;

import java.util.List;

public class TreeTraverseTest {

	public static void main(String[] args) {
		TreeNode root = new TreeNode(1);
		root.left = new TreeNode(2);
		root.right = new TreeNode(3);
		root.left.left = new TreeNode(4);
		root.left.right = new TreeNode(5);
		root.right.left = new TreeNode(6);
		root.right.right = new TreeNode(7);
		
		TraverseAlgorithm ta = new TraverseAlgorithm();
		/*
		ta.preOrderTraverse(root);
		System.out.println();
		ta.preOrderTraverseByRecursive(root);
		*/
		/*
		ta.inOrderTraverse(root);
		System.out.println();
		ta.inOrderTraverseByRecursive(root);
		*/
		/*
		ta.postOrderTraverse(root);
		System.out.println();
		ta.postOrderTraverseByRecursive(root);
		*/
		
		List<List<Integer>> res1 = ta.levelOrder(root);
		List<List<Integer>> res2 = ta.levelOrder2(root);
		System.out.println("test");
		
	}

}
