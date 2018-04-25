import java.util.*;
import java.util.stream.Collectors;

class Vertex {
    String name, color;
    int time, value, mark;
    Set<Vertex> to, from, parent;

    public Vertex(String name, int time) {
        this.name = name;
        this.color = "black";
        this.time = this.value = time;
        this.mark = -1;
        this.to = new HashSet<>();
        this.from = new HashSet<>();
        this.parent = new HashSet<>();
    }

    public void calculate() {
        if (this.time == this.value && this.from.size() > 0) {
            this.from.stream().forEach(Vertex::calculate);
            Vertex x = Collections.max(this.from, (a, b) -> a.value - b.value);
            this.parent.addAll(this.from.stream().filter(u -> u.value == x.value).collect(Collectors.toList()));
            this.value += x.value;
        }
    }

    public void link(Vertex v) {
        this.to.add(v);
        v.from.add(this);
    }

    public void makeBlue() {
        this.color = "blue";
        this.to.stream().filter(u -> !u.color.equals("blue")).forEach(Vertex::makeBlue);
    }

    public void makeRed() {
        this.color = "red";
        this.parent.forEach(Vertex::makeRed);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;
        if (o == null || getClass() != o.getClass())
            return false;
        Vertex vertex = (Vertex) o;
        return value == vertex.value;
    }

    @Override
    public String toString() {
        String ans = "\t" + this.name + "[label = \"" + this.name + "(" + this.time + ")\"";
        if (!this.color.equals("black")) {
            ans += ", color = " + this.color;
        }
        return ans + "]";
    }
}

public class Cpm {
    private static void dfs(Vertex v, List<Vertex> dst) {
        v.mark = 0;
        for (Vertex u : v.to) {
            if (u.mark == -1) {
                dfs(u, dst);
            } else if (u.mark == 0) {
                dst.add(u);
            }
        }
        v.mark = 1;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        Map<String, Vertex> m = new Hashtable<>();
        for (String previous = null; in.hasNext(); ) {
            String s = in.next();
            boolean last = false;
            if (!s.equals("<")) {
                if (s.charAt(s.length() - 1) == ';') {
                    s = s.substring(0, s.length() - 1);
                    last = true;
                }
                int l = s.lastIndexOf('(');
                if (l != -1) {
                    int value = Integer.parseInt(s.substring(l + 1, s.length() - 1));
                    s = s.substring(0, l);
                    m.put(s, new Vertex(s, value));
                }
                if (previous != null) {
                    m.get(previous).link(m.get(s));
                }
                previous = s;
                if (last) {
                    previous = null;
                }
            }
        }
        List<Vertex> blue = new ArrayList<>();
        m.values().stream().filter(v -> v.mark == -1).forEach(v -> dfs(v, blue));
        blue.stream().forEach(Vertex::makeBlue);
        m.values().stream().filter(v -> !v.color.equals("blue")).forEach(Vertex::calculate);
        Optional<Vertex> o = m.values().stream().filter(v -> !v.color.equals("blue"))
                .max((a, b) -> a.value - b.value);
        Vertex x = o.orElse(null);
        List<Vertex> red = new ArrayList<>(m.values().stream()
                .filter(v -> !v.color.equals("blue"))
                .filter(u -> u.equals(x))
                .collect(Collectors.toList()));
        red.stream().forEach(Vertex::makeRed);
        System.out.println("digraph {");
        m.values().stream().forEach(System.out::println);
        for (Vertex v : m.values()) {
            for (Vertex u : v.to) {
                System.out.print("\t" + v.name + " -> " + u.name);
                if (v.color.equals("blue") || (v.color.equals("red") && u.color.equals("red") && u.value == v.value + u.time)) {
                    System.out.print(" [color = " + v.color + "]");
                }
                System.out.println();
            }
        }
        System.out.println("}");
    }
}
