use std::time::{Duration, Instant};

const N: usize = 500;
const L: f32 = 100.0;
const LOOP: usize = 1;

fn init_data(data: &mut [[f32; N]; N]) {
    let mut rng = rand::thread_rng();
    for i in 0..N {
        for j in i..N {
            data[i][j] = rand::random::<f32>() * L;
        }
    }
    for i in 0..N-1 {
        for j in i+1..N {
            for k in 0..N {
                data[j][k] += data[i][k];
            }
        }
    }
}

fn init_matrix(data: &[[f32; N]; N], matrix: &mut [[f32; N]; N]) {
    for i in 0..N {
        for j in 0..N {
            matrix[i][j] = data[i][j];
        }
    }
}

fn calculate_serial(matrix: &mut [[f32; N]; N]) {
    for k in 0..N {
        for j in k+1..N {
            matrix[k][j] /= matrix[k][k];
        }
        matrix[k][k] = 1.0;
        for i in k+1..N {
            for j in k+1..N {
                matrix[i][j] -= matrix[i][k] * matrix[k][j];
            }
            matrix[i][k] = 0.0;
        }
    }
}

fn main() {
    let mut data: [[f32; N]; N] = [[0.0; N]; N];
    let mut matrix: [[f32; N]; N] = [[0.0; N]; N];

    init_data(&mut data);

    let mut time = Duration::new(0, 0);
    for _ in 0..LOOP {
        init_matrix(&data, &mut matrix);
        let start = Instant::now();
        calculate_serial(&mut matrix);
        let duration = start.elapsed();
        time += duration;
    }
    
    println!("serial: {:.2}ms", (time.as_micros() as f32) / 1000.0 / (LOOP as f32));
}
