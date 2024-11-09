#include <iostream>
#include <random>
#include <fstream>

//интерфейс для разных случайных величин
class DiceIntrf
{
public:
    virtual unsigned roll() = 0;
};

//дискр. случайная величина равномерно распред. от 1 до max
class Dice: public DiceIntrf
{

private:

    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;

public:

    Dice(unsigned max, unsigned seed): max(max), dstr(1, max), reng(seed) {}

    unsigned roll()
    {
        return dstr(reng);
    }

};

//случайная величина равная сумме 3-х некоторых случайных величин
class ThreeDicePool: public DiceIntrf
{

private:

    DiceIntrf &dice_1, &dice_2, &dice_3;

public:

    ThreeDicePool(DiceIntrf &dice_1, DiceIntrf &dice_2, DiceIntrf &dice_3): dice_1(dice_1), dice_2(dice_2), dice_3(dice_3) {}

    unsigned roll()
    {
        return dice_1.roll() + dice_1.roll() + dice_1.roll();
    }
};

//случайная величина равная минимальному значению из 2-х значений некоторой случайной величины
class PenaltyDice: public virtual DiceIntrf
{

    private:

        DiceIntrf &dice;

    public:

        PenaltyDice(DiceIntrf &dice): dice(dice) {}

        unsigned roll()
        {
            return std::min(dice.roll(), dice.roll());
        }
};

//случайная величина равная максимальному значению из 2-х значений некоторой случайной величины
class BonusDice: public virtual DiceIntrf
{

    private:

        DiceIntrf &dice;

    public:

        BonusDice(DiceIntrf &dice): dice(dice) {}

        unsigned roll()
        {
            return std::max(dice.roll(), dice.roll());
        }
};

//случайная величина равная сумме минимального значения из 2-х значений некоторой случайной величины и максимального значения из 2-х других значений некоторой случайной величины
class DoubleDice: public PenaltyDice, public BonusDice
{
    private:

    public:
        DoubleDice(DiceIntrf &dice): PenaltyDice(dice), BonusDice(dice) {}

        unsigned roll()
        {
            return PenaltyDice::roll() + BonusDice::roll();
        }


};

/*class DoubleDice: public DiceIntrf
{
    private:

        DiceIntrf &dice;

    public:
        DoubleDice(DiceIntrf &dice): dice(dice) {}

        unsigned roll()
        {
            auto a = dice.roll();
            auto b = dice.roll();
            auto c = dice.roll();
            auto d = dice.roll();

            return (std::min(a,b)+std::max(c,d));
        }


};
*/

//функция вычисляющая мат ожидание некоторой случайной величины
double expected_value(DiceIntrf &d, unsigned number_of_rolls = 1000)
{
    auto accum = 0llu;
    for(unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += d.roll();
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

//функция вычисляющая вероятность попадания некоторой случайной величины в задонное значение
double value_probabitity(unsigned value, DiceIntrf &d, unsigned number_of_rolls = 1)
{
    auto sucsess = 0llu;
    for(unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
    {
        if(d.roll() == value)
            sucsess++;
    }

    return
        static_cast<double>(sucsess) / static_cast<double>(number_of_rolls);
}

int main()
{
    setlocale(LC_ALL, "Rus");
    unsigned rolls = 19999;


    //задание 1
    Dice dice(100, 111);
    Dice dice1(6, 256), dice2(6, 352), dice3(6, 82);
    ThreeDicePool threedice(dice1, dice2, dice3);
    std::cout << "Мат. ожидание дискр. сл. величины равномерно распред. от 1 до 100: " << expected_value(dice, rolls) << std::endl
    << "Мат. ожидание суммы 3-х дискр. сл. величин равномерно распред. от 1 до 6: " << expected_value(threedice, rolls)<< std::endl;


    //задание 2
    std::ofstream out("data_lab1.txt");
    PenaltyDice pendice(dice);
    BonusDice bondice(dice);
    std::cout<<"Мат. ожидание пенальти: "<<expected_value(pendice, rolls)<<std::endl<<"Мат. ожидание бонуса: "<<expected_value(bondice, rolls)<< std::endl;
    out<<"dice"<<"\n";
    for(unsigned cnt = 1; cnt <= 100; ++cnt)
    {
        //std::cout<<value_probabitity(cnt, dice, rolls)<<std::endl;
        out<<value_probabitity(cnt, dice, rolls)<<"\n";
    }
    //std::cout<<std::endl;
    out<<"\n";
    out<<"pendice"<<"\n";
    for(unsigned cnt = 1; cnt <= 100; ++cnt)
    {
        //std::cout<<value_probabitity(cnt, pendice, rolls)<<std::endl;
        out<<value_probabitity(cnt, pendice, rolls)<<"\n";
    }
    //std::cout<<std::endl;
    out<<"\n";
    out<<"bondice"<<"\n";
    for(unsigned cnt = 1; cnt <= 100; ++cnt)
    {
        //std::cout<<value_probabitity(cnt, bondice, rolls)<<std::endl;
        out<<value_probabitity(cnt, bondice, rolls)<<"\n";
    }
    //std::cout<<std::endl;
    out<<"\n";
    out<<"threedice"<<"\n";
    for(unsigned cnt = 3; cnt <= 18; ++cnt)
    {
        //std::cout<<value_probabitity(cnt, threedice, rolls)<<std::endl;
        out<<value_probabitity(cnt, threedice, rolls)<<"\n";
    }
    //std::cout<<std::endl;
    out<<"\n";


    //задание 3
    DoubleDice doubdice(dice);
    std::cout<< "Мат. ожидание суммы пенальти и бонуса: "<<expected_value(doubdice, rolls)<<std::endl;
    out<<"doubdice"<<"\n";
    for(unsigned cnt = 1; cnt <= 200; ++cnt)
    {
        //std::cout<<value_probabitity(cnt, doubdice, rolls)<<std::endl;
        out<<value_probabitity(cnt, doubdice, rolls)<<"\n";
    }
    //std::cout<<std::endl;
    out<<"\n";


    out.close();

    return 0;

}

