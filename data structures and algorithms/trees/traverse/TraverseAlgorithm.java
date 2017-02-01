package algorithm.tree;

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
	
	public void preOrderTraverse(){
//		Stack<TreeNode> 
	}
	
	public void inOrderTraverseByRecursive(TreeNode root){
		if(null != root){
			inOrderTraverseByRecursive(root.left);
			visitNode(root);
			inOrderTraverseByRecursive(root.right);
		}
	}
	
	public void postOrderTraverseByRecursive(TreeNode root){
		if(null != root){
			visitNode(root);
			postOrderTraverseByRecursive(root.left);
			postOrderTraverseByRecursive(root.right);
		}
	}
	
	
}
