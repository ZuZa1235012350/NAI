#include <cmath>
#include <array>
#include <functional>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>
std::random_device random_dev;
std::default_random_engine random_engine(random_dev());
std::uniform_real_distribution<double> double_random_dist(0.0, 1.0);
std::uniform_int_distribution<char> rand_01(0, 1);

using Gene = bool;
using Chromosome = std::array<Gene, 128>;

uint64_t BinaryToGray(uint64_t num)
{
    return num ^ (num >> 1);
}

void intoArray(uint64_t val, Chromosome &bits, int start)
{
    for (int i = 0; i < 64; i++)
    {
        bits[start + i] = (val >> (64 - 1 - i)) & 1;
    }
}
uint64_t toUint(const Chromosome &bits, int start)
{
    uint64_t num = 0;
    for (uint64_t i = 0; i < 64; i++)
    {
        uint64_t bit_val = bits[start+i];
        num |= (bit_val << (64 - 1 - i));
    }
    return num;
}

uint64_t GrayToBinary(uint64_t num)
{
    uint64_t mask = num;
    while (mask) {
        mask >>= 1;
        num ^= mask;
    }
    return num;
}

double schaffer(double x, double y)
{
    double nominator = std::pow(std::cos(std::sin(std::abs(x*x-y*y))),2) - 0.5;
    double denominator = std::pow(1 + 0.001*(x*x + y*y),2);
    return 0.5 + nominator/denominator;
}

std::pair<double, double> toFenotypeSchaffer(const Chromosome &chromosome)
{
    uint64_t x,y;
    x = toUint(chromosome, 0);
    x = GrayToBinary(x);
    y = toUint(chromosome, 64);
    y = GrayToBinary(y);
    double max_val = (double)std::numeric_limits<uint64_t>::max();
    double xd = -100 + x/max_val * 200.0;
    double yd = -100 + y/max_val * 200.0;
    return std::make_pair(xd, yd);
}

Chromosome toGenotypeSchaffer(double xd, double yd)
{
    Chromosome chromosome;
    double max_val = (double)std::numeric_limits<uint64_t>::max();
    uint64_t x = (uint64_t)((xd + 100) / 200.0 * max_val);
    uint64_t y = (uint64_t)((yd + 100) / 200.0 * max_val);
    x = BinaryToGray(x);
    y = BinaryToGray(y);
    intoArray(x, chromosome, 0);
    intoArray(y, chromosome, 64);
    return chromosome;
}

double schaffer_fitness(const Chromosome &chromosome)
{
    auto [x, y] = toFenotypeSchaffer(chromosome);
    double epsilon = 0;
    return 1/(schaffer(x,y)+epsilon);
}

auto test_fun = schaffer;
auto fitness_fun = schaffer_fitness;
auto to_fenotype = toFenotypeSchaffer;
auto to_genotype = toGenotypeSchaffer;

struct Individual
{
    double fitness;
    double x,y,val;
    Chromosome chromosome;
    Individual(Chromosome chromosome) : chromosome(std::move(chromosome))
    {
        fitness = fitness_fun(this->chromosome);
        auto [x, y] = to_fenotype(this->chromosome);
        this->x = x;
        this->y = y;
        this->val = test_fun(x,y);
    }
};

using Population = std::vector<Individual>;

Population selectionRoulette(Population population)
{
    Population new_population;
    std::vector<double> cumulative_fitness;
    double total_fitness = 0.0;
    for(auto& individual : population)
    {
        total_fitness += individual.fitness;
        cumulative_fitness.push_back(total_fitness);
    }
    std::uniform_real_distribution<double> random_dist(0.0, total_fitness);
    for(int i = 0; i < population.size(); i++)
    {
        double random_fitness = random_dist(random_engine);
        int index = std::distance(cumulative_fitness.begin(), std::lower_bound(cumulative_fitness.begin(), cumulative_fitness.end(), random_fitness));
        new_population.push_back(population[index]);
    }
    return new_population;
}

std::pair<Chromosome, Chromosome> crossover(const Chromosome &a, const Chromosome &b)
{
    std::pair<Chromosome, Chromosome> children;
    for(int i = 0; i < a.size(); i++)
    {
        if(rand_01(random_engine) == 0)
        {
            children.first[i] = a[i];
        }
        else
        {
            children.first[i] = b[i];
        }
        if(rand_01(random_engine) == 0)
        {
            children.second[i] = a[i];
        }
        else
        {
            children.second[i] = b[i];
        }
    }
    return children;
}

Chromosome mutation(const Chromosome &a, double probability)
{
    Chromosome mutated_chromosome;
    for(int i = 0; i < a.size(); i++)
    {
        if(double_random_dist(random_engine) < probability)
        {
            mutated_chromosome[i] = 1 - a[i];
        }
        else
        {
            mutated_chromosome[i] = a[i];
        }
    }
    return mutated_chromosome;
}

Population genetic_algorithm(
        std::function<double(const Chromosome&)> fitness_f,
        const Population &init_population,
        double crossover_probability,
        double mutation_probability,
        std::function<std::pair<Chromosome, Chromosome>(const Chromosome&, const Chromosome&)> crossover_f,
        std::function<Chromosome(const Chromosome&, double)> mutation_f,
        std::function<Population(const Population&)> select_f,
        std::function<bool(const Population&, int)> termination_cond)
{
    int iteration = 0;
    Population population = init_population;

    while (termination_cond(population, iteration)) {
        auto offspring_pop = select_f(population);

        for (int i = 1; i < offspring_pop.size(); i += 2) {
            auto parent0 = offspring_pop[i - 1];
            auto parent1 = offspring_pop[i];
            if (double_random_dist(random_engine) < crossover_probability) {
                auto [a, b] = crossover_f(parent0.chromosome, parent1.chromosome);
                offspring_pop[i - 1] = Individual(a);
                offspring_pop[i] = Individual(b);
            }
        }

        for (int i = 0; i < offspring_pop.size(); i++) {
            offspring_pop[i] = Individual(mutation_f(offspring_pop[i].chromosome, mutation_probability));
        }

        population = Population{offspring_pop};

        iteration++;
    }

    return population;
}

int main()
{
    double mutation_probability = 0.001;
    double crossover_probability = 0.5;
    const int pop_size = 200;
    double min = -100, max = 100;
    double len = max - min;
    Population init_pop;
    for (int i = 0; i < pop_size; ++i)
    {
        double val = min+i*len/pop_size;
        Chromosome ch = to_genotype(val, val);
        init_pop.push_back(Individual(ch));
    }
    std::cout << "Starting population\n";
    for(const auto &ind : init_pop)
    {
        std::cout << "(" << ind.x << ", " << ind.y << ")" <<
            ": " << ind.val <<
            " - fitness:  " << ind.fitness << std::endl;
    }
    int iter_count = 100000;
    auto result = genetic_algorithm(fitness_fun, init_pop,
        crossover_probability, mutation_probability,
        crossover, mutation, selectionRoulette,
        [iter_count](const auto&, auto i){return i < iter_count;});
    std::cout << "End population in order\n";
    std::sort(result.begin(), result.end(), [](auto a, auto b){return a.fitness > b.fitness;});
    for(const auto &ind : result)
    {
        std::cout << "(" << ind.x << ", " << ind.y << ")" <<
            ": " << ind.val <<
            " - fitness:  " << ind.fitness << std::endl;
    }
    return 0;
}