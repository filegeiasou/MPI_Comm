# Parallel_mathematics

Σας ζητείται να φτιάξετε ένα MPI πρόγραμμα (σε γλώσσα C και θεωρώντας point-to-point
communication), το οποίο, δοθέντος (ως είσοδο) ενός διανύσματος X (μήκους n στοιχείων xi | 
i=0…n-1), **να υπολογίζει παράλληλα** σε περιβάλλον ‘p’ επεξεργαστών και να τυπώνει στην
οθόνη (ως έξοδο) τα ακόλουθα:

- (α) τη μέση τιμή (μ) των στοιχείων του διανύσματος X:
  - μ= (x0+x1+x2+…+xn-1) / n
- (β) τη μέγιστη τιμή (m) των στοιχείων του διανύσματος X:
  - xi ισχύει ότι xi <= m και ∃ xi τέτοιο ώστε xi = m
- (γ) τη διασπορά (var) των στοιχείων του διανύσματος X:
  - var = ((x0–μ)^2+(x1–μ)^2+(x2–μ)^2+…+(xn-1–μ)^2) / n
- (δ) ένα νέο διάνυσμα Δ (μήκους n στοιχείων δi | i=0…n-1), του οποίου κάθε στοιχείο δi θα
ισούται με τo τετράγωνο της απόλυτης τιμής της διαφοράς του αντίστοιχου στοιχείου (xi) του
διανύσματος Χ από τη μέγιστη τιμή m:
  - δi = |xi – m|^2

**Χρησιμοποιήστε μόνο τις απλές συναρτήσεις επικοινωνίας τύπου MPI_Send()/MPI_Recv()**
(μην χρησιμοποιήσετε συναρτήσεις συλλογικής επικοινωνίας). Το σύνολο του απαιτούμενου
υπολογιστικού φόρτου θα πρέπει να ισοκατανεμηθεί στους ‘p’ επεξεργαστές του παράλληλου
περιβάλλοντός σας. Επίσης, κάθε επεξεργαστής θα πρέπει να λαμβάνει (κατέχει) στην τοπική
του μνήμη μόνο τα δεδομένα εισόδου που χρησιμοποιεί για τοπικούς (δικούς του)
υπολογισμούς. Τις τιμές/στοιχεία ‘n’, ‘xi’ θα πρέπει να τις δίνει ο χρήστης και να τις διαβάζει
αντίστοιχα από την οθόνη ένας από τους ‘p’ επεξεργαστές του παράλληλου περιβάλλοντός σας
(π.χ. ο επεξεργαστής ‘0’ τον οποίον για αυτό το λόγο τον θεωρούμε ως τον ‘κεντρικό'
επεξεργαστή του παράλληλου περιβάλλοντος). Τα αποτελέσματα του προγράμματός σας θα
πρέπει επίσης να συγκεντρώνονται στο τέλος στον ‘κεντρικό’ αυτό επεξεργαστή και μέσω
αυτού να παρουσιάζονται στο χρήστη.
Αναπτύξτε τον κώδικά σας παραμετρικά ώστε να δουλεύει σωστά για οποιονδήποτε αριθμό
πολλαπλών επεξεργαστών ‘p’, θεωρώντας ότι το ‘n’ είναι ακέραιο πολλαπλάσιο του ‘p’. Θα
πρέπει επίσης το πρόγραμμά σας να δουλεύει (α) με menu επιλογών (π.χ. 1. Συνέχεια – 2.
Έξοδος) και (β) επαναληπτικά (δηλαδή να εμφανίζει επαναληπτικά το παραπάνω menu μέχρι
να επιλέξει ο χρήστης την επιλογή ‘εξόδου’ από το πρόγραμμα).
