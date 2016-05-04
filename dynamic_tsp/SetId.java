public class SetId {
    int size;
    int subsetSize;
    int bitString;
    int count;
    int countMax;
    SetId(int subsetSize, int size){
        bitString = 0;
        this.size = size;
        this.subsetSize = subsetSize;
        count = 0;
        countMax = (int)(factorial(size) / (factorial(size - subsetSize)*factorial(subsetSize)));
    }
    public boolean hasNext(){
        return count < countMax;
    }

    public TSPSet next(){
        count++;
        while(! BitsActive(subsetSize, size, bitString)){
            bitString++;
        }
        bitString++;
        return new TSPSet(bitString-1, size, subsetSize);
    }
    private static boolean BitsActive(int bitsActive, int size, int bitString) {
        int activeCount = 0;
        for (int counter = 0; counter < size; counter++) {
            if ((bitString & Bitmask.mask[counter]) != 0) {
                activeCount++;
            }
        }
        return activeCount == bitsActive;
    }
    // we cud also print a number to a certain index,
    // so 31b to bit index 2.
    static void intToBinary(int dec){
        String bitString = "";
        for(int counter = 0; counter < 16; counter ++){
            bitString = dec%2 + bitString;
            dec = dec/2;
        }
        System.out.println(bitString);
    }
    public static long factorial(long n) {
        if      (n <  0) throw new RuntimeException("Underflow error in factorial");
        else if (n > 20) throw new RuntimeException("Overflow error in factorial");
        else if (n == 0) return 1;
        else             return n * factorial(n-1);
    }
}
