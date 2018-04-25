import java.lang.Integer;
import java.lang.System;
import java.util.*;
import java.util.ArrayList;



class Vertex {
    public int T1 = 0, comp = -1, low, number;

    Vertex(int number) {
        this.number = number;
    }

    Vertex() {
        number = 0;
    }

    public String toString() {
        return Integer.toString(number);
    }

    public List<Vertex> to = new ArrayList<Vertex>();
}

public class GraphBase {


    private static Scanner in = new Scanner(System.in);
    private static int time = 1, count = 0;

    private static void VisitVertex_Tarjan(Vertex v, Deque<Vertex> myStack) {
        v.T1 = v.low = time++;
        myStack.push(v);
        for (Vertex tmp : v.to) {
            if (tmp.T1 == 0)
                VisitVertex_Tarjan(tmp, myStack);
            if (tmp.comp == -1 && tmp.low < v.low)
                v.low = tmp.low;
        }

        if (v.T1 == v.low) {
            do {
                myStack.peekFirst().comp = count;
            } while (myStack.pop() != v);
            count++;
        }
    }

    public static void main(String[] args) {
        int c, n = in.nextInt(), i = in.nextInt();
        List<Vertex> graph = new ArrayList<Vertex>(n);
        for (c = 0; c < n; graph.add(new Vertex(c++))) {}
        for (c = 0; c < i; c++) {
            graph.get(in.nextInt()).to.add(graph.get(in.nextInt()));
        }
        Deque<Vertex> myStack = new ArrayDeque<Vertex>();

        for(Vertex e: graph)
            if(e.T1 == 0)
                VisitVertex_Tarjan(e, myStack);
        Set<Integer>[] g = (Set<Integer>[]) new Set[count + 1];
        for (i = 0; i <= count; g[i++] = new HashSet<Integer>()) {}
        for (Vertex v : graph) {
            for (Vertex u : v.to) {
                if (u.comp != v.comp) {
                    g[u.comp].add(v.comp);
                }
            }
        }
        SortedSet<Integer> ans = new TreeSet<>();
        for (i = 0; i < count; i++) {
            if (g[i].size() == 0) {
                for (Vertex v : graph) {
                    if (v.comp == i) {
                        ans.add(v.number);
                        break;
                    }
                }
            }
        }
        for (int x : ans) {
            System.out.print(x + " ");
        }
    }
}
