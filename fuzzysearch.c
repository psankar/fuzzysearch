/*
 * Fuzzy Search:
 *
 * Search for the string needle in a fuzzy way inside the string haystack.
 * Resultant words are displayed where each boundaries of each word is
 * identified by the delimiters array.
 *
 * To build and run:
 * gcc -Wall -Wextra -Werror fuzzysearch.c -o fuzzysearch
 * ./fuzzysearch
 *
 * All the code is available in Creative Commons Zero License.
 * More information at: http://creativecommons.org/publicdomain/zero/1.0/
 *
 * If you looking for some ideas to improve this code, you can try to make this
 * a library with a proper .h file and define APIs. You can also get the results
 * sorted based on the number_of_characters_matching. You can also use a sqlite
 * in-memory database to speed up and simplify a lot of operaitons. These will
 * be easy fixes but I am not focusing on these as the main idea behind implementing
 * this is to get an idea of fuzzy search and I have got that.
 */

#include <stdio.h>
#include <stdlib.h>

/* Turn Debug output on/off */
#if 0
#define d(x) printf(x)
#else
#define d(x)
#endif

typedef struct _match_result {
	int word_beginning_offset;
	int word_ending_offset;	/* We can avoid this field and iterate until we reach a delimiter too */
	int first_matching_letter_offset_within_word;
	int number_of_characters_matching;
} match_result;

/* We can do more interesting things like, moving this list to an in-memory
* sqlite database and the above struct as each record in the database and
* thereby simplifying operations like searching / sorting / caching largely */
typedef struct _match_results {
	match_result *result;
	struct _match_results *next;
} match_results;

int main()
{
	char haystack[] = "Sufficiently long text here with enough characters\n\
	    g_widget_new\tg_gadget_new   g_matching_olf\n\
	    g_figure_new  ggw";

	char needle[] = "ggw";

	char delimiter[] = "\n\t ";

	int i;			/* The haystack iterator */
	int j;			/* The delimiter iterator */
	int k = 0;		/* The needle iterator */
	int word_beginning_offset;
	match_result *result = NULL;
	match_results *results_list = NULL;

	d("About to calculate \n");

	for (i = 0; haystack[i] != '\0'; i++) {
		for (j = 0; delimiter[j] != '\0'; j++) {
			if (haystack[i] == delimiter[j]) {
				/* The current character is a delimitter, so
				 * skip and go to the next word */
				word_beginning_offset = i + 1;
				if (result) {
					d("Added new result to the list\n");
					result->word_ending_offset = i;
					/* Prepend the new result to the results_list
					 * that we maintain */
					match_results *results_entry =
					    malloc(sizeof(match_results));
					results_entry->result = result;
					results_entry->next = results_list;
					results_list = results_entry;

					/* These two will change only a result
					 * has been found and so can be safely
					 * kept inside this loop, instead of the
					 * outer if loop */
					result = NULL;
					k = 0;
				}
				continue;
			}
		}		/* End of delimiter iteration */

		if (haystack[i] == needle[k]) {
			if (k == 0) {
				/* First character in needle matches with a
				 * character in the current word for the first time */
				result = malloc(sizeof(match_result));
				result->word_beginning_offset =
				    word_beginning_offset;
				result->first_matching_letter_offset_within_word
				    = i;
				result->number_of_characters_matching = 1;
				d("First character matched in a word\n");
			} else {
				result->number_of_characters_matching++;
				d("One more character matched for the same word\n");
			}
			k++;
		}		/* End of haystack, needle comparison block */
	}			/* End of haystack iteration */

	/* If the last word in the haystack had a match, prepend it to the list of
	 * results that we maintain */
	if (result) {
		d("Last word of the haystack too had a match\n");

		result->word_ending_offset = i;

		match_results *results_entry = malloc(sizeof(match_results));
		results_entry->result = result;
		results_entry->next = results_list;
		results_list = results_entry;

		/* Setting this to NULL so that there will not be a dangling
		 * reference when we free all the elements of the
		 * match_results later */
		result = NULL;
	}

	/* Print the results */
	d("Printing the results\n");
	while (results_list) {

		for (i = results_list->result->word_beginning_offset;
		     i < results_list->result->word_ending_offset; i++) {
			printf("%c", haystack[i]);
		}
		printf("\t%d",
		       results_list->result->number_of_characters_matching);

		/* We could free the old result. But it is not needed unless we develop
		 * this as a library and expose these functions. Now since we will exit
		 * the program, we are avoiding the free of each result */
		results_list = results_list->next;
		printf("\n");
	}

	return 0;
}
