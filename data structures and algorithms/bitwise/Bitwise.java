package test;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Bitwise {

	/*
	 * 下面代码取自
	 * https://discuss.leetcode.com/topic/50315/a-summary-how-to-use-bit-manipulation-to-solve-problems-easily-and-efficiently
	 * 
	 * 
	WIKI

	Bit manipulation is the act of algorithmically manipulating bits or other pieces of data shorter than a word. Computer programming tasks that require bit manipulation include low-level device control, error detection and correction algorithms, data compression, encryption algorithms, and optimization. For most other tasks, modern programming languages allow the programmer to work directly with abstractions instead of bits that represent those abstractions. Source code that does bit manipulation makes use of the bitwise operations: AND, OR, XOR, NOT, and bit shifts.

	Bit manipulation, in some cases, can obviate or reduce the need to loop over a data structure and can give many-fold speed ups, as bit manipulations are processed in parallel, but the code can become more difficult to write and maintain.

	DETAILS

	Basics

	At the heart of bit manipulation are the bit-wise operators & (and), | (or), ~ (not) and ^ (exclusive-or, xor) and shift operators a << b and a >> b.

	There is no boolean operator counterpart to bitwise exclusive-or, but there is a simple explanation. The exclusive-or operation takes two inputs and returns a 1 if either one or the other of the inputs is a 1, but not if both are. That is, if both inputs are 1 or both inputs are 0, it returns 0. Bitwise exclusive-or, with the operator of a caret, ^, performs the exclusive-or operation on each pair of bits. Exclusive-or is commonly abbreviated XOR.
		Set union A | B
		Set intersection A & B
		Set subtraction A & ~B
		Set negation ALL_BITS ^ A or ~A
		Set bit A |= 1 << bit
		Clear bit A &= ~(1 << bit)
		Test bit (A & 1 << bit) != 0
		Extract last bit A&-A or A&~(A-1) or x^(x&(x-1))
		Remove last bit A&(A-1)
		Get all 1-bits ~0
	
	Examples
	*/
	
	/*
	 * & tricks
	 */
	
	/*
	 * Count the number of ones in the binary representation of the given number
	 * 
	 * 计算一个int数中有多少个二进制1，即Hamming weight问题。最快的算法就是下面这个，
	 * 时间复杂度：O(m) ，即无符号整形中（二进制）1的个数)  
	 * 
	 * 直观思路如下：
	 *  while(num) { 
		    if((num & 1) != 0) {   // 时间复杂度：O(log n) ，右移位运算就是除以2) 
		        count ++; 
		    } 
		    num = num>>>1; 
 		}  
	 * 
	 */
	int getHammmingWeight(int n) {
		int count = 0;
	    while(n != 0) {
	        n = n&(n-1);
	        count++;
	    }
	    return count;
	}
	
	/*
	 * Is power of four (actually map-checking, iterative and recursive methods can do the same)
	 * 判断n是否为4的乘方（幂）
	 * 
	 * 问题描述参见https://leetcode.com/problems/power-of-four/ leetcode中的power of four问题
	 * 循环或是递归的时间复杂度应该是O(logn)
	 * 
	 * 不用循环或递归，那么就考虑位运算
	 * 		从二进制考虑，4的幂肯定也是2的幂，但根据4的幂的特点
	 * 		1			1
	 * 		4		  100
	 * 		16		10000
	 * 		64	  1000000
	 * 		可以得出4的幂中1必然出现在奇数位置，因此用0x55555555进行
	 * 		与运算，排除那些是2的幂、不是4的幂的数，例如32（100000，
	 * 		2的5次方，但01010101与之后等于0，排除掉）
	 */
	boolean isPowerOfFour(int num) {
	    return num > 0 && ((num&(num-1)) == 0) && ((num & 0x55555555) != 0);
	    //check the 1-bit location;
	}
	
	
	/*
	 * ^ tricks
	 * Use ^ to remove even exactly same numbers and save the odd, or save the distinct bits and remove the same.
	 * 
	 */

	/*
	 * SUM OF TWO INTEGERS 
	 * Use ^ and & to add two integers
	 * https://leetcode.com/problems/sum-of-two-integers/
	 * 
	 * 非递归实现
	public int getSum(int a, int b) {
        if(a == 0) return b;
		if(b == 0) return a;
        int carry = 0;
        while(b != 0){
//        	 这一步算出了二进制加法中a和b相同位置都是1的情况，即所有的进位，
//        	 进位算出后，carry左移一位就是进位加到对应位置
        	carry = a & b;
        	
//        	 a^b计算了a和b中相同位置数字不相同，即一个是1、另一个是0的情况，
//        	 两个都是1的情况已通过上面a&b算出，两个是0的情况对加法没有影响，
//        	 也不用考虑。
        	a = a ^ b;
        	b = carry << 1;
        }
//         在carry=0时说明已经没有进位，a ^ b已经是最终的结果
        return a;
    }
	 * 
	 */
	int getSum(int a, int b) {
	    return b==0? a:getSum(a^b, (a&b)<<1); //be careful about the terminating condition;
	}
	
	/*
	 * MISSING NUMBER
	 * 
	 * Given an array containing n distinct numbers taken from 0, 1, 2, ..., n, find the one that is missing from the array. For example, Given nums = [0, 1, 3] return 2. (Of course, you can do this by math.)
	 * 
	 * https://leetcode.com/problems/missing-number/
	 */
	int missingNumber(int[] nums) {
		int xor = 0, i = 0;
    	for (i = 0; i < nums.length; i++) {
    		xor = xor ^ i ^ nums[i];
    	}
    	return xor ^ i;
	}
	
	/*
	 * | tricks
	 */
	
	/*
	 * Keep as many 1-bits as possible
	 * 
	 * Find the largest power of 2 (most significant bit in binary form), which is less than or equal to the given number N.
	 * 获取最贴近N的2的幂，Integer.highestOneBit(int n)内部实现即为这种算法，
	 * 通过不断右移获取到的n是一个从最高位开始全是1的数，减去不带符号位右移1位的n，
	 * 就减去了所有除最高位1之外的所有1
	 * 分析参见http://blog.csdn.net/jessenpan/article/details/9617749 Java Integer.highestOneBit(i)代码品读
	 * 注意java中左移、右移带符号、不带符号的区别。
	 */
	int getLargestPower(int n) {
	    //changing all right side bits to 1.
	    n = n | (n>>1);
	    n = n | (n>>2);
	    n = n | (n>>4);
	    n = n | (n>>8);
	    n = n | (n>>16);
//	    return (n+1)>>1;//这样做理论上可以，但存在溢出风险，因此下面采用下面的n-n>>>1
	    return n - n >>> 1;
	}
	
	
	/*
	 * REVERSE BITS
	 * 
	 * Reverse bits of a given 32 bits unsigned integer.
	 * 
	 * https://leetcode.com/problems/reverse-bits/
	 * 
	 * 
	 */
	public int reverseBits(int n) {
		int res = 0;
    	for(int i = 0; i < 32; i++){
    		res = res << 1;
    		res |= n & 1;
    		n = n >>> 1;
    	}
    	return res;
    }
	
	
	/*
	 * & tricks
	 * 
	 */
	
	/*
	 * Just selecting certain bits
	 * 
	 * C++ version https://discuss.leetcode.com/topic/9811/o-1-bit-operation-c-solution-8ms
	 * 
	uint32_t reverseBits(uint32_t n) {
        n = (n >> 16) | (n << 16);
        n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
        n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
        n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
        n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
        return n;
    }
	 * 
	 */
	
	/*
	 * Bitwise AND of Numbers Range
	 * Given a range [m, n] where 0 <= m <= n <= 2147483647, return the bitwise AND of all numbers in this range, inclusive. 
	 * For example, given the range [5, 7], you should return 4.
	 * 
	 * https://leetcode.com/problems/bitwise-and-of-numbers-range/
	 */
	
	/*


	int rangeBitwiseAnd(int m, int n) {
	    int a = 0;
	    while(m != n) {
	        m >>= 1;
	        n >>= 1;
	        a++;
	    }
	    return m<<a; 
	}
	*/
	
	
	/*
	 * NUMBER OF 1 BITS
	 * 
	 * Write a function that takes an unsigned integer and returns the number of ’1' bits it has (also known as the Hamming weight).

	Solution(C++ version)

	int hammingWeight(uint32_t n) {
		int count = 0;
		while(n) {
			n = n&(n-1);
			count++;
		}
		return count;
	}
	int hammingWeight(uint32_t n) {
	    ulong mask = 1;
	    int count = 0;
	    for(int i = 0; i < 32; ++i){ //31 will not do, delicate;
	        if(mask & n) count++;
	        mask <<= 1;
	    }
	    return count;
	}
	 * 
	 * */
	
	
	/*
	 * Application
	 * 
	 * */
	
	/*
	 * REPEATED DNA SEQUENCES
	 * 
	 * All DNA is composed of a series of nucleotides abbreviated as A, C, G, and T, for example: "ACGAATTCCG". When studying DNA, it is sometimes useful to identify repeated sequences within the DNA. Write a function to find all the 10-letter-long sequences (substrings) that occur more than once in a DNA molecule.	
	 * For example,	
	 * Given s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT",
	 * Return: ["AAAAACCCCC", "CCCCCAAAAA"].
	 * 
	 * https://leetcode.com/problems/repeated-dna-sequences/
	 * 
	 * Solution(C++ version)
	 
	class Solution {
	public:
	    vector<string> findRepeatedDnaSequences(string s) {
	        int sLen = s.length();
	        vector<string> v;
	        if(sLen < 11) return v;
	        char keyMap[1<<21]{0};
	        int hashKey = 0;
	        for(int i = 0; i < 9; ++i) hashKey = (hashKey<<2) | (s[i]-'A'+1)%5;
	        for(int i = 9; i < sLen; ++i) {
	            if(keyMap[hashKey = ((hashKey<<2)|(s[i]-'A'+1)%5)&0xfffff]++ == 1)
	                v.push_back(s.substr(i-9, 10));
	        }
	        return v;
	    }
	};
	But the above solution can be invalid when repeated sequence appears too many times, in which case we should use unordered_map<int, int> keyMap to replace char keyMap[1<<21]{0}here.	

	 * 
	 * Solution(Java version)
	 * https://discuss.leetcode.com/topic/8894/clean-java-solution-hashmap-bits-manipulation
	 */
	public List<String> findRepeatedDnaSequences(String s) {
        List<String> res = new ArrayList<String>();
		if(null == s) return res;
		Set<Integer> words = new HashSet<Integer>();
		Set<Integer> doubleWords = new HashSet<Integer>();
		int[] codeMap = new int[26];
		//codeMap['A'-'A'] = 0; is default
		codeMap['C'-'A'] = 1;
		codeMap['G'-'A'] = 2;
		codeMap['T'-'A'] = 3;
		for(int i = 0; i < s.length() - 9; i++){
			int code = 0;
			for(int j = i; j < i+10; j++){
				code <<= 2;
				code |= codeMap[s.charAt(j) - 'A'];
			}
			if(!words.add(code) && doubleWords.add(code)){
				res.add(s.substring(i, i+10));
			}
		}
        return res;
    }
	
	/*
	 * MAJORITY ELEMENT
	 * Given an array of size n, find the majority element. The majority element is the element that appears more than ⌊ n/2 ⌋ times. (bit-counting as a usual way, but here we actually also can adopt sorting and Moore Voting Algorithm)
	 * 
	 * https://leetcode.com/problems/majority-element/
	 * 
	 * Solution explanations 
	 * https://discuss.leetcode.com/topic/17446/6-suggested-solutions-in-c-with-explanations
	 * Another nice idea! The key lies in how to count the number of 1's on a specific bit. Specifically, you need a mask with a 1 on the i-the bit and 0 otherwise to get the i-th bit of each element in nums. The code is as follows.
	 * 
	int majorityElement(vector<int>& nums) {
        int major = 0, n = nums.size();
        for (int i = 0, mask = 1; i < 32; i++, mask <<= 1) {
            int bitCounts = 0;
            for (int j = 0; j < n; j++) {
                if (nums[j] & mask) bitCounts++;
                if (bitCounts > n / 2) {
                    major |= mask;
                    break;
                }
            }
        } 
        return major;
    } 
	 * 
	 * https://discuss.leetcode.com/topic/28601/java-solutions-sorting-hashmap-moore-voting-bit-manipulation
	 * */
	// Bit manipulation 
	public int majorityElement(int[] nums) {
	    int[] bit = new int[32];
	    for (int num: nums)
	        for (int i=0; i<32; i++) 
	            if ((num>>(31-i) & 1) == 1)
	                bit[i]++;
	    int ret=0;
	    for (int i=0; i<32; i++) {
	        bit[i]=bit[i]>nums.length/2?1:0;
	        ret += bit[i]*(1<<(31-i));
	    }
	    return ret;
	}
	
	
	/*
	 * SINGLE NUMBER
	 * Given an array of integers, every element appears three times except for one, which appears exactly once. Find that single one.
	 * https://leetcode.com/problems/single-number-ii/
	 * 

	//inspired by logical circuit design and boolean algebra;
	//counter - unit of 3;
	//current   incoming  next
	//a b            c    a b
	//0 0            0    0 0
	//0 1            0    0 1
	//1 0            0    1 0
	//0 0            1    0 1
	//0 1            1    1 0
	//1 0            1    0 0
	//a = a&~b&~c + ~a&b&c;
	//b = ~a&b&~c + ~a&~b&c;
	//return a|b since the single number can appear once or twice;
	int singleNumber(vector<int>& nums) {
	    int t = 0, a = 0, b = 0;
	    for(int i = 0; i < nums.size(); ++i) {
	        t = (a&~b&~nums[i]) | (~a&b&nums[i]);
	        b = (~a&b&~nums[i]) | (~a&~b&nums[i]);
	        a = t;
	    }
	    return a | b;
	}
	 * 
	 * */
	
	
	/*


	MAXIMUM PRODUCT OF WORD LENGTHS

	Given a string array words, find the maximum value of length(word[i]) * length(word[j]) where the two words do not share common letters. You may assume that each word will contain only lower case letters. If no such two words exist, return 0.

	Example 1:
	Given ["abcw", "baz", "foo", "bar", "xtfn", "abcdef"]
	Return 16
	The two words can be "abcw", "xtfn".
	Example 2:
	Given ["a", "ab", "abc", "d", "cd", "bcd", "abcd"]
	Return 4
	The two words can be "ab", "cd".
	Example 3:
	Given ["a", "aa", "aaa", "aaaa"]
	Return 0
	No such pair of words.
	Solution

	Since we are going to use the length of the word very frequently and we are to compare the letters of two words checking whether they have some letters in common:

	using an array of int to pre-store the length of each word reducing the frequently measuring process;
	since int has 4 bytes, a 32-bit type, and there are only 26 different letters, so we can just use one bit to indicate the existence of the letter in a word.
	int maxProduct(vector<string>& words) {
	    vector<int> mask(words.size());
	    vector<int> lens(words.size());
	    for(int i = 0; i < words.size(); ++i) lens[i] = words[i].length();
	    int result = 0;
	    for (int i=0; i<words.size(); ++i) {
	        for (char c : words[i])
	            mask[i] |= 1 << (c - 'a');
	        for (int j=0; j<i; ++j)
	            if (!(mask[i] & mask[j]))
	                result = max(result, lens[i]*lens[j]);
	    }
	    return result;
	}
	Attention

	result after shifting left(or right) too much is undefined
	right shifting operations on negative values are undefined
	right operand in shifting should be non-negative, otherwise the result is undefined
	The & and | operators have lower precedence than comparison operators
	SETS

	All the subsets
	A big advantage of bit manipulation is that it is trivial to iterate over all the subsets of an N-element set: every N-bit value represents some subset. Even better, if A is a subset of B then the number representing A is less than that representing B, which is convenient for some dynamic programming solutions.

	It is also possible to iterate over all the subsets of a particular subset (represented by a bit pattern), provided that you don’t mind visiting them in reverse order (if this is problematic, put them in a list as they’re generated, then walk the list backwards). The trick is similar to that for finding the lowest bit in a number. If we subtract 1 from a subset, then the lowest set element is cleared, and every lower element is set. However, we only want to set those lower elements that are in the superset. So the iteration step is just i = (i - 1) & superset.

	vector<vector<int>> subsets(vector<int>& nums) {
	    vector<vector<int>> vv;
	    int size = nums.size(); 
	    if(size == 0) return vv;
	    int num = 1 << size;
	    vv.resize(num);
	    for(int i = 0; i < num; ++i) {
	        for(int j = 0; j < size; ++j)
	            if((1<<j) & i) vv[i].push_back(nums[j]);   
	    }
	    return vv;
	}
	Actually there are two more methods to handle this using recursion and iteration respectively.

	BITSET

	A bitset stores bits (elements with only two possible values: 0 or 1, true or false, ...).
	The class emulates an array of bool elements, but optimized for space allocation: generally, each element occupies only one bit (which, on most systems, is eight times less than the smallest elemental type: char).

	// bitset::count
	#include <iostream>       // std::cout
	#include <string>         // std::string
	#include <bitset>         // std::bitset

	int main () {
	  std::bitset<8> foo (std::string("10110011"));
	  std::cout << foo << " has ";
	  std::cout << foo.count() << " ones and ";
	  std::cout << (foo.size()-foo.count()) << " zeros.\n";
	  return 0;
	}
	*/
}
