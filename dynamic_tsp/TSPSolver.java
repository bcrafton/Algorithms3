import javafx.util.Pair;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

public class TSPSolver {
    private int tspTable[][];
    private ArrayList<Integer> table[][];
    private int size;
    private int setCount;
    public TSP tsp;

    TSPSolver(String filename, int size) throws IOException{
        tsp = new TSP(filename, size);
        this.size = size;
        this.setCount =  (1 << (size-1));
        tspTable = new int[size][setCount];
        table = new ArrayList[size][setCount];
        for(int i = 0; i < size; i++){
            for(int j = 0; j < setCount; j++){
                table[i][j] = new ArrayList<>();
            }
        }
    }
    public void setIndex(int row, int set, int val){
        tspTable[row][set] = val;
    }
    public int solve(){
        for(int problemSize = 1; problemSize < size; problemSize++){
            TSPSet set;
            SetId s = new SetId(problemSize, size-1);
            while(s.hasNext())
            {
                set = s.next();
                Iterator<Integer> itr = set.setIterator();
                while(itr.hasNext())
                {
                    int lastVertex = itr.next();
                    // we are not going to use index 0 for vertex 1 since vertex 1 is not con
                    setIndex(lastVertex, set.setId, getDistance(lastVertex, set));
                }
            }
        }
        int minValue = 0;
        int value;
        int minIndex = 0;
        boolean first = true;
        for(int counter = 1; counter < size-1; counter++){
            value = getIndex(counter, setCount-1) + tsp.getDistance(counter, 0);
            if(minValue > value || first){
                minValue = value;
                first = false;
                minIndex = counter;
            }
        }
        Iterator<Integer> itr = table[minIndex][setCount-1].iterator();
        System.out.println(1);
        while(itr.hasNext()){
            System.out.println(itr.next()+1);
        }
        System.out.println(1);
        return minValue;
    }
    public int getIndex(int row, int set){
        return tspTable[row][set];
    }
    // this tables values will all include the distance to get to index 1.
    public int getDistance(int start, TSPSet set){
        if(set.setSize == 1){
            this.table[start][set.setId].add(start);
            return tsp.getDistance(0, start);
        }
        else {
            // for each element in the set, not j, find min value of g(j, k, set)
            boolean first = true;
            int minValue = 0;
            int minNext = 0;
            int value;
            Iterator<Integer> itr = set.setIterator();
            while(itr.hasNext()){
                int next = itr.next();
                if(next != start)
                {
                    value = tsp.getDistance(start, next) + getIndex(next, set.remove(start).setId);//getDistance(next, new TSPSet(set.setId & ~Bitmask.mask[start], set.size, set.setSize-1));
                    if((value < minValue) || first){
                        minValue = value;
                        minNext = next;
                        first = false;
                    }
                }
            }
            table[start][set.setId] = new ArrayList<>(table[minNext][set.remove(start).setId]);
            table[start][set.setId].add(start);
            return minValue;
        }
    }
}

