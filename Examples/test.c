void print(char string[]){}


int main()
{
    double number, sum = 0;

    // the body of the loop is executed at least once
    do
    {
        number = 3;

        if (sum > 15){
        	print("a");
        } else if (sum > 20){
        	print("s");
        }
        else {
        	print("d");
        }

        sum += number;
    }
    while(number != 0.0);

    print("Sum");

    return 0;
}