#include "graph.h"
#include "brute_strong.h"


StrongAntimagicBruteParams::StrongAntimagicBruteParams(AntimagicBruteParams *abp, int increment_max) {
    this->abp = abp;
    this->increment_max = increment_max;
}


void StrongAntimagicBruteParams::print_stat(bool same_line) {
    printf("\rChecked: %i Not strong antimagic: %i%s",
           (int) abp->checked,
           (int) abp->non_antimagic,
           same_line ? "" : "\n");
}

void StrongAntimagicBruteParams::print_stat_inline() {
    if (abp->checked % 10 == 0) {
        abp->mutex_print.lock();
        print_stat(true);
        fflush(stdout);
        abp->mutex_print.unlock();
    }
}


uint worker_strong_antimagic(void* arg, string line) {
    auto * wa = (WorkerArg*) arg;
    auto * sabp = (StrongAntimagicBruteParams*) wa->bp;
    auto * abp = sabp->abp;

    Graph g(line);

    bool connected = g.is_connected();
    connected ? abp->connected++ : abp->not_connected++;

    if (abp->check_only_not_connected && !connected)
        return WORKER_RETURN_OKAY;

    bool is_strong_antimagic = true;
    for (int increment = 0; increment <= sabp->increment_max; ++increment)
        if (!g.is_antimagic(increment)) {
            is_strong_antimagic = false;
            break;
        }
    abp->checked++;

    if (is_strong_antimagic) {
        abp->antimagic++;
        connected ? abp->connected_antimagic++ : abp->not_connected_antimagic++;
    }
    else {
        abp->non_antimagic++;
        connected ? abp->connected_non_antimagic++ : abp->not_connected_non_antimagic++;

        if (abp->write_not_antimagic) {
            abp->mutex_vec.lock();
            abp->vec_not_antimagic.push_back(line);
            abp->mutex_vec.unlock();
        }
    }

    sabp->print_stat_inline();

    return WORKER_RETURN_OKAY;
}


void worker_strong_antimagic_finalize(void* arg) {
    auto * wa = (WorkerArg*) arg;
    auto * sabp = (StrongAntimagicBruteParams*) wa->bp;
    auto * abp = sabp->abp;
    sabp->print_stat(false);
    printf("Maximum increment: %i\n", sabp->increment_max);
    printf("Connected all: %i\n"
           "Connected and possible strong antimagic: %i\n"
           "Connected and not strong antimagic: %i\n",
           (int) abp->connected,
           (int) abp->connected_antimagic,
           (int) abp->connected_non_antimagic);
    printf("Not connected: %i\n"
           "Not connected and possible strong antimagic: %i\n"
           "Not connected and not antimagic: %i\n",
           (int) abp->not_connected,
           (int) abp->not_connected_antimagic,
           (int) abp->not_connected_non_antimagic);
    if (abp->write_not_antimagic)
        write_to_file("not_antimagic.txt", abp->vec_not_antimagic);
}
