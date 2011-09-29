;; returns cons cell whose car points to 'COOP or 'DEFECT and the 
;; cdr points to the history structure
(defun decide-aanz (opID history last-op-decision opchange mychange)
  ; last-op-desc to ophist (ophist now in sync)
  ; opchange, mychange to probdata
  ; make decision
  ; current-decision to ophist (ophist out of sync)
  ; return decision and history


; update ophist data with last-op-decision, TODO fix for nil input, should not create table
(defun ophist-op-update-aanz (opid ophist last-op-decision)
  (let ((entry (assoc opid ophist)))
    (progn 
      (if (not entry) (progn (setf entry (list opid nil nil) (setf ophist (cons entry ophist)))))
      (setf (second entry) (cons last-op-decision (second entry))))))

; assumes ophist in sync (when looking up decisions made last round)
; update probdata with opchange, mychange, (does nothing on the first round against an opponent, ie when an entry doesnt exist in ophist)
(defun probdata-update-aanz (opid ophist probdata opchange mychange)
  (let ((entry (assoc opid ophist)))
    (if entry (let ((mychoice (first (second entry))) (opchoice (first (third entry))))
      ; mychoice, opchoice should not be nil, since if so, entry should also be nil
      (cond ((and (eq mychoice 'c) (eq opchoice 'c)) (setf (first probdata) (sort (cons opchange (cons mychange (first probdata))) #'<)))
            ((and (eq mychoice 'c) (eq opchoice 'd)) (setf (second probdata) (sort (cons mychange (second probdata)) #'<)))
            ((and (eq mychoice 'd) (eq opchoice 'c)) (setf (second probdata) (sort (cons opchange (second probdata)) #'<)))
            ((and (eq mychoice 'd) (eq opchoice 'd)) (setf (third probdata) (sort (cons opchange (cons mychange (third probdata))) #'<))))))))



  (if (assoc opid ophist) 
  (setf (ophist (second (assoc opid ophist))) (cons last-op-decision (second (assoc (opid ophist))))) 
        (myhist (third (assoc opid ophist)))
        ; revise all this...

;                            ophist --------------------------          probdata-----------------
; history of form '(('ophist ( ( 1 '( c d ..)) (2 '(d c ..)) ) ) ('data (( 80 90 ..) ( 50 60 ..)) ))

;; history is opponent specific and returns c or d
;; history of form ('c 'd 'd ...) most recent first
(defun make-choice-aanz (func ophistory)
  (if (< (rand-aanz) (make-choice-prob-aanz func ophistory)) 'c 'd))

;; helper for make choice, (gives probablity of cooperating)
(defun make-choice-prob-aanz (func ophistory)
  (let ((c-prob (second func)) (func-op (cddr func)))
    (progn (dotimes (i (length func-op)) 
      (progn (if (eq (first ophistory) 'd) (setf c-prob (+ c-prob (first func-op))))
             (setf func-op (rest func-op))
             (setf ophistory (rest ophistory))))
           (- 1 c-prob))))

; returns a function (in its list form)
(defun look-up-aanz (prob-data)
  (single-look-up-aanz 

)

; looks up the function for each of the expected values cc cd dd
(defun single-look-up-aanz (cc cd dd) 
  (assoc dd (assoc cd (assoc cc data))))
 

;; returns a real number which is the value of the probability
;; distribution evaluated at x
;; assumes prob-data is sorted
(defun prob-aanz (prob-data mychoice opchoice x)
  (let ((d (cond ((and (eq mychoice 'c) (eq opchoice 'c)) (first prob-data))
                 ((and (eq mychoice 'c) (eq opchoice 'd)) (second prob-data))
                 ((and (eq mychoice 'd) (eq opchoice 'd)) (third prob-data))
                 (T nil))) 
        (bot (cond ((and (eq mychoice 'c) (eq opchoice 'c)) 80)
                 ((and (eq mychoice 'c) (eq opchoice 'd)) 0)
                 ((and (eq mychoice 'd) (eq opchoice 'd)) 40)
                 (T 100))) 
        (top (cond ((and (eq mychoice 'c) (eq opchoice 'c)) 100)
                 ((and (eq mychoice 'c) (eq opchoice 'd)) 100)
                 ((and (eq mychoice 'd) (eq opchoice 'd)) 90)
                 (T 100)))
        last)
    ; sort here if data unsorted
    (or (dolist (a d)
      (if (> a x) (return (/ (if last 1.0 .5) (* (length d) (- a (or last bot))))) (setf last a)))
        (if last (/ .5 (* (length d) (- top last))) (/ 1.0 (- top bot))))))


; returns random number between 0 and 1
(defun rand-aanz () 
  (/ (random 4294967296) 4294967296.0))
