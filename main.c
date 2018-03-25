/* btree in userspace, shamelessly copied from linux kernel.
 *
 * btree_init   void btree_init(struct btree_head *head)
 * btree_insert int btree_insert(struct btree_head *head, struct btree_geo *geo, unsigned long *key, void *val)
 * btree_lookup void *btree_lookup(struct btree_head *head, struct btree_geo *geo, unsigned long *key)
 * btree_last unsigned long *btree_last(struct btree_head *head, struct btree_geo *geo)
 * btree_merge int btree_merge(struct btree_head *target, struct btree_head *victim, struct btree_geo *geo, unsigned long *duplicate)
 * btree_remove void *btree_remove(struct btree_head *head, struct btree_geo *geo, unsigned long *key)
 * btree_visitor  size_t btree_visitor(struct btree_head *head, struct btree_geo *geo,
 *                       long opaque,
 *                       void (*func)(void *elem, long opaque, unsigned long *key,
 *                       size_t index, void *func2), void *func2);
 * btree_grim_visitor size_t btree_grim_visitor(struct btree_head *head, struct btree_geo *geo,
 *                           long opaque,
 *                           void (*func)(void *elem, long opaque, unsigned long *key,
 *                           size_t index, void *func2), void *func2)
 * visitor128 void visitor128(void *elem, long opaque, unsigned long *__key, size_t index, void *__func);
 * visitor32 void visitor32(void *elem, long opaque, unsigned long *__key, size_t index, void *__func);
 * visitor64 void visitor64(void *elem, long opaque, unsigned long *__key, size_t index, void *__func);
 * visitorl void visitorl(void *elem, long opaque, unsigned long *key, size_t index, void *__func);
 *
 * BTREE TESTER:
 * create, lookup, distroy.
 * 
 * struct btree_head {
 *      unsigned long *node;
 *      int height;
 * };
 *
 * struct btree_geo {
 *      int keylen;
 *      int no_pairs;
 * };
 *
 * static inline int btree_insert64(struct btree_head64 *head, u64 key, void *val)
 * static inline void *btree_lookup64(struct btree_head64 *head, u64 key)
 * static inline void *btree_remove64(struct btree_head64 *head, u64 key)
 * static inline u64 btree_last64(struct btree_head64 *head)
 *
 */
#include "btree.h"
#include <time.h>

int main(void)
{
	int idx;
	struct btree_head64 btree;

	srand(time(NULL));
	btree_init64(&btree);
	for (idx = 0; idx < 10; idx++) {
		int *p = malloc(sizeof(int));
		*p = rand()%1000;
		printf("%p\n", p);
		printf("%d\n", btree_insert64(&btree, idx, p));
		printf("%d\n", *(int *)btree_lookup64(&btree, idx));
//		printf("%d\n", *(int *)btree_remove64(&btree, idx));
	}
	printf("height %d\n", btree.h.height);
	dumptree(&btree.h, &btree_geo64);
	printf("%d %d\n",btree_geo64.keylen, btree_geo64.no_pairs);
//	free(p);
	return 0;
}
