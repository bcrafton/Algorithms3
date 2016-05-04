import java.io.IOException;

public class main {
    public static void main(String [] args)
    {
        try{
            TSPSolver tsp = new TSPSolver("p01_d.txt", 15);
            tsp.tsp.print();
            System.out.print(tsp.solve());
        } catch(IOException e){
            System.out.print("IOException\n");
        }
        /**
         * This code works but its total garbage.
         * The idea is:
         * For each subset size:
         *      For each set in subset set
         *              For each vertex v in set
         *                      For each vertex u in set != v
         *                          g(v, u, set-{v)) = min(Cvu + lookup table [u][setid]);
         * return minValue of subset size size-1 + Cindex,1
         *
         */

        /**
         * So basically u have n arrays of 2^n length
         * u go through filling up the smallest subsets
         * then using those subsets to fill up the larger subsets
         * with -> g(v, u, set-{v)) = min(Cvu + lookup table [u][setid]);
         * i shud have already calculated lookup table [u][setid]
         * when i did g(u, ?, set-{u))
         */
    }
}
