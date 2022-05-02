import sys
import numpy as np


class Food:
    def __init__(self, string):
        self._ingredients = []
        self._allergens = []

        split = string.split(" ")
        i = 0
        while i < len(split) and split[i] != "(contains":
            self._ingredients.append(split[i])
            i += 1

        i += 1
        while i < len(split):
            self._allergens.append(split[i][:-1])
            i += 1


def list_all_ingredients_and_allergens(foods):
    ingredients_set, allergens_set = set(), set()
    for food in foods:
        for ingredient in food._ingredients:
            ingredients_set.add(ingredient)
        for allergen in food._allergens:
            allergens_set.add(allergen)

    return list(ingredients_set), list(allergens_set)


def find_safe_ingredients(ingredients, allergens, foods):
    safe_ingredients = []
    for ingredient in ingredients:
        foods_without = filter(lambda f: ingredient not in f._ingredients, foods)
        allergens_in_foods_without = list_all_ingredients_and_allergens(
            foods_without
        )[1]
        if len(allergens_in_foods_without) == len(allergens):
            safe_ingredients.append(ingredient)

    return safe_ingredients


def match_allergens_to_ingredients(ingredients, allergens, foods):
    foods_without_ingredient = [
        [ingredient not in f._ingredients for f in foods] for ingredient in ingredients
    ]
    foods_with_allergen = [
        [allergen in f._allergens for f in foods] for allergen in allergens
    ]
    m = np.empty([len(ingredients), len(allergens)], dtype=np.bool)
    for r in range(m.shape[0]):
        for c in range(m.shape[1]):
            m[r, c] = not np.any(
                np.logical_and(foods_without_ingredient[r], foods_with_allergen[c])
            )

    d = {}
    while len(d) < len(allergens):
        for r in range(m.shape[0]):
            hits = np.where(m[r, :])[0]
            if len(hits) == 1:
                d[allergens[hits[0]]] = ingredients[r]
                m[:, hits[0]] = False

    return d


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        data = f.read().splitlines()

    foods = [Food(d) for d in data]
    ingredients, allergens = list_all_ingredients_and_allergens(foods)
    safe_ingredients = find_safe_ingredients(ingredients, allergens, foods)

    if sys.argv[2] == "1":
        out = 0
        for food in foods:
            for ingredient in safe_ingredients:
                if ingredient in food._ingredients:
                    out += 1
    elif sys.argv[2] == "2":
        dangerous_ingredients = list(
            filter(lambda i: i not in safe_ingredients, ingredients)
        )
        allergens_dict = match_allergens_to_ingredients(
            dangerous_ingredients, allergens, foods
        )
        out = ",".join([allergens_dict[allergen] for allergen in sorted(allergens)])

    print(out)
