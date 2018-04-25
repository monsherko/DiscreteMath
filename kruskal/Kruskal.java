import java.util.*;

class Edge {
    private Vertex v, u;
    private float length;

    public static class Vertex {
        private Vertex parent = this;
        private int depth = 0;
        private int x, y;

        Vertex(int a, int b) {
            x = a; y = b;
        }

        public int x() {
            return x;
        }

        public int y() {
            return y;
        }

        public Vertex find() {
            if (this != parent) {
                parent = parent.find();
            }
            return parent;
        }

        public void union(Vertex v) {
            Vertex a = find(), b = v.find();
            if (a.depth < b.depth) {
                a.parent = b;
            } else {
                b.parent = a;
                if (a.depth == b.depth && a != b) {
                    a.depth++;
                }
            }
        }
    }

    Edge(Vertex a, Vertex b) {
        v = a; u = b;
        length = (float) Math.sqrt((v.x() - u.x()) * (v.x() - u.x()) + (v.y() - u.y()) * (v.y() - u.y()));
    }

    public float length() {
        return length;
    }

    public Vertex from() {
        return v;
    }

    public Vertex to() {
        return u;
    }
}

public class Kruskal {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt(), j, i, top;
        double result = 0;
        Edge.Vertex[] g = new Edge.Vertex[n];
        Edge.Vertex v, u;
        for (i = 0; i < n; i++) {
            g[i] = new Edge.Vertex(in.nextInt(), in.nextInt());
        }
        List<Edge> q = new ArrayList<>(n * (n - 1) / 2);
        for (i = 0, top = 0; i < n; i++) {
            for (j = i + 1; j < n; j++) {
                q.add(new Edge(g[i], g[j]));
            }
        }
        Collections.sort(q, new Comparator<Edge>() {
            public int compare(Edge a, Edge b) {
                if (a.length() != b.length()) {
                    return a.length() > b.length() ? 1 : -1;
                }
                return 0;
            }
        });
        for (Edge e : q) {
            if ((v = e.from()).find() != (u = e.to()).find()) {
                result += e.length();
                v.union(u);
            }
        }
        System.out.format(Locale.US, "%.2f", result);
    }
}
