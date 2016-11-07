 //typedef float (*T)(float, int);
// START CHALLENGE

float do_math(float arg1, int arg2) {
    return arg2 / arg1;
}

float call_a_func(T call_this) {
    float output = call_this(5.5, 7);
    return output;
}

int main() {
  float final_result = call_a_func(&do_math);
}
