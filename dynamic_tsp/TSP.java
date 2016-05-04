import java.io.File;
import java.io.IOException;
import java.util.Scanner;

/**
 * Created by Brian on 9/7/2015.
 */
public class TSP {
    private int size;
    private int distanceTable[][];

    TSP(String filename, int size) throws IOException{
        File file = new File(filename);
        Scanner s = new Scanner(file);
        this.size = size;
        distanceTable = new int[size][size];

        int rowCounter = 0;
        int indexCounter = 0;
        while(s.hasNext() && rowCounter < size){
            distanceTable[rowCounter][indexCounter] = s.nextInt();
            indexCounter++;
            if(indexCounter == size){
                rowCounter++;
                indexCounter = 0;
            }
        }
    }
    // only works up to 32 cities, can use byte arrays after that
    // so we are going to have size# bitstrings of size length
    // 000...000 to 111...111 size times ... n2^n
    public int getDistance(int row, int index){
        return distanceTable[row][index];
    }
    public void print(){
        for(int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                System.out.print(getDistance(i, j)+ " ");
            }
            System.out.println();
        }
    }
}

