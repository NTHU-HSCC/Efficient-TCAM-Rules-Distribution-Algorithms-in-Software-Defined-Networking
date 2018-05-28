Readme file for TableDecomposition (2018/03/13)

1. Click the file named "TableDecomposition" to open the project.

2. In this project, there are 3 files, "main.cpp", "TOPOLOGY.cpp", and "TOPOLOGY.H"

3. the file "TOPOLOGY.H" defines serveral constants, functions and structures.  

   PATHNUM: the maximum number of flows.

   SWITCHNUM: the maximum number of switches.

   LONGESTPATH: the maximum length of the flows.

   FILENUM: the number of testing topologies.

   SLEN: the minimum length of the flows.

   MAX_M: the maximum number of colors

4. In the file "main.cpp", 

    the 2D-array "cnt" records the sum of the number of colors of each iteration.

    	cnt[0][sLen]  records the sum of the number of colors of 1-Greedy

    	cnt[1][sLen]  records the sum of the number of colors of 2-Greedy

    	cnt[2][sLen]  records the sum of the number of colors of SA

    	cnt[3][sLen]  records the sum of the number of colors of Optimal

    the 2D-array "m_counter" records the number of the results that there are j colors.

    	m_counter[0][j]: the number of the results which there are exacly j colors for 1-Greedy 

    	m_counter[1][j]: the number of the results which there are exacly j colors for 2-Greedy

    	m_counter[2][j]: the number of the results which there are exacly j colors for SA

    	m_counter[3][j]: the number of the results which there are exacly j colors for Optimal


5.	the steps of simulations

	I. Set the constants in "TOPOLOGY.H" according to the requirements.

	II. Build and compile the file "main.cpp".

    	line 122 of "main.cpp" will  print out the results of that topology and the average number of colors.