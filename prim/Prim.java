import java.util.*;


class Vertex {
    int index = -1, key = -1;
    List<Map.Entry<Vertex, Integer>> to = new ArrayList<Map.Entry<Vertex, Integer>>();
}

public class Prim {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt(), m = in.nextInt(), result = 0;
        Vertex[] g = new Vertex[n];
        for (int i = 0; i < n; g[i] = new Vertex(), i++) {}
        for (int i = 0; i < m; i++) {
            int x = in.nextInt(), y = in.nextInt(), z = in.nextInt();
            g[x].to.add(new AbstractMap.SimpleEntry<>(g[y], z));
            g[y].to.add(new AbstractMap.SimpleEntry<>(g[x], z));
        }
        Queue<Vertex> q = new PriorityQueue<>((a, b) -> Integer.compare(a.key, b.key));
        for (Vertex v = g[0];;) {
            v.index = -2;
            for (Map.Entry<Vertex, Integer> e : v.to) {
                if (e.getKey().index == -1) {
                    e.getKey().key = e.getValue(); e.getKey().index = 1;
                    q.add(e.getKey());
                }
                else if (e.getKey().index != -2 && e.getValue() < e.getKey().key) {
                    q.remove(e.getKey()); e.getKey().key = e.getValue(); q.add(e.getKey());
                }
            }
            if (q.size() == 0)
                break;
            result += (v = q.poll()).key;
        }
        System.out.println(result);
    }
}
