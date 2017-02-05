package algorithm.tree;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

public class GraphAlgorithm {

	/*
	 * 
	 * http://www.cnblogs.com/developerY/p/3323264.html
	 * 
	 */
	private static void bfs(HashMap<Character, LinkedList<Character>> graph, HashMap<Character, Integer> dist,
			char start) {
		Queue<Character> q = new LinkedList<>();
		q.add(start);// 将s作为起始顶点加入队列
		dist.put(start, 0);
		int i = 0;
		while (!q.isEmpty()) {
			char top = q.poll();// 取出队首元素
			i++;
			System.out.println("The " + i + "th element:" + top + " Distance from s is:" + dist.get(top));
			int d = dist.get(top) + 1;// 得出其周边还未被访问的节点的距离
			for (Character c : graph.get(top)) {
				if (!dist.containsKey(c))// 如果dist中还没有该元素说明还没有被访问
				{
					dist.put(c, d);
					q.add(c);
				}
			}
		}
	}

	static int count = 0;

	private static void dfs(HashMap<Character, LinkedList<Character>> graph, HashMap<Character, Boolean> visited) {
		visit(graph, visited, 'u');// 为了和图中的顺序一样，我认为控制了DFS先访问u节点
		visit(graph, visited, 'w');
	}

	private static void visit(HashMap<Character, LinkedList<Character>> graph, HashMap<Character, Boolean> visited,
			char start) {
		if (!visited.containsKey(start)) {
			count++;
			System.out.println("The time into element " + start + ":" + count);// 记录进入该节点的时间
			visited.put(start, true);
			for (char c : graph.get(start)) {
				if (!visited.containsKey(c)) {
					visit(graph, visited, c);// 递归访问其邻近节点
				}
			}
			count++;
			System.out.println("The time out element " + start + ":" + count);// 记录离开该节点的时间
		}
	}
}
