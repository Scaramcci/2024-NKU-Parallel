use std::fs::File;
use std::io::{Write, BufWriter};
use std::time::Instant;
use rayon::prelude::*;
use packed_simd_2::f32x4;

const L: f32 = 100.0;
const LOOP: usize = 50;

fn init_data(n: usize) -> Vec<Vec<f32>> {
    let mut data = vec![vec![0.0; n]; n];
    let mut rng = rand::thread_rng();
    for i in 0..n {
        for j in i..n {
            data[i][j] = rand::random::<f32>() * L;
        }
    }
    for i in 0..n-1 {
        for j in i+1..n {
            for k in 0..n {
                data[j][k] += data[i][k];
            }
        }
    }
    data
}

fn calculate_serial(matrix: &mut [Vec<f32>]) {
    let n = matrix.len();
    for k in 0..n {
        for j in k+1..n {
            matrix[k][j] /= matrix[k][k];
        }
        matrix[k][k] = 1.0;
        for i in k+1..n {
            for j in k+1..n {
                matrix[i][j] -= matrix[i][k] * matrix[k][j];
            }
            matrix[i][k] = 0.0;
        }
    }
}

fn main() {
    let mut res_stream = BufWriter::new(File::create("result.csv").unwrap());
    for i in (100..=1000).step_by(100).chain((1000..=3000).step_by(500)) {
        test(i, &mut res_stream);
    }
}

fn test(n: usize, res_stream: &mut impl Write) {
    writeln!(res_stream, "Size: {}", n).unwrap();
    let mut data = init_data(n);
    let mut matrix = data.clone();
    let start = Instant::now();
    calculate_serial(&mut matrix);
    let duration = start.elapsed();
    writeln!(res_stream, "Serial: {:.2?}", duration).unwrap();
}
