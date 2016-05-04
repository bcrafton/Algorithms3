import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by Brian on 9/9/2015.
 */
public class TSPSet {
    int setId;
    int size;
    int setSize;

    TSPSet(int setId, int size, int setSize){
        this.setId = setId;
        this.size = size;
        this.setSize = setSize;
    }
    Iterator<Integer> setIterator(){
        List<Integer> list = new LinkedList<>();
        for(int counter = 0; counter < size; counter++){
            if((this.setId & Bitmask.mask[counter]) != 0){
                list.add(counter+1);
            }
        }
        return list.iterator();
    }
    TSPSet remove(int element){
        return new TSPSet(setId & ~Bitmask.mask[element-1], size, setSize-1);
    }
}
